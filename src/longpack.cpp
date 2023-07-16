/*
 * Snapmaker2-Modules library
 *
 * Based on Snapmaker2-Modules
 * (see https://github.com/Snapmaker/Snapmaker2-Modules)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "longpack.h"

uint16_t CalcChecksum(uint8_t *data, uint16_t len) {
    uint32_t checksum = 0;
    for (int i = 0; i < len -1; i = i + 2) {
        checksum += ((data[i] << 8) | data[i + 1]);
    }

    if (len % 2) {
        checksum += data[len - 1];
    }

    while (checksum > 0xffff) {
        checksum = ((checksum >> 16) & 0xffff) + (checksum & 0xffff);
    }
    checksum = ~checksum;

    return checksum;
}


void Longpack::Init(MCP_CAN *canBus, uint32_t txId) {
    can = canBus;
    tx_id = txId;
}


ERR_E Longpack::readLongpack(uint8_t value) {
    if (recv_index_ == 0 && value == MAGIC_PART_1) {
        // parse started
        packData_[recv_index_++] = value;
    } else if (recv_index_ == 1 && value != MAGIC_PART_2) {
        // wrong data, skip
        recv_index_ = 0;
    } else if (recv_index_ > 0) {
        packData_[recv_index_++] = value;
        if (recv_index_ == 6) {
            // len_high(bit 2) concat len_low(bit 3)  = len_check(bit 5)
            if ((packData_[2] ^ packData_[3]) != packData_[5]) {
                // wrong data, skip
                recv_index_ = 0;
            }
        } else if (recv_index_ >= MAX_SYS_CMD_LEN) {
            // probably firmware update, we don't need it
            cmd_clean();
            return E_FALSE;
        } else if (recv_index_ > 6) {
            uint16_t dataFieldLen = packData_[2] << 8 | packData_[3];
            if (dataFieldLen + sizeof(PackHead) == recv_index_) {
                packData_[recv_index_] = 0;
                recv_index_ = 0;
                // reach the end of the pack
                uint8_t * dataFiled = packData_ + sizeof(PackHead); // skip the packhead
                uint16_t checksum = CalcChecksum(dataFiled, dataFieldLen);

                // len_check_high(bit 6) concat len_check_low(bit 7) were calculated by caller.
                // This check will avoid most data corruption.
                if (checksum == ((packData_[6] << 8) | packData_[7])) {
                    return E_TRUE;
                } else {
                    cmd_clean();
                    return E_FALSE;
                }
            }
        }
        // if all above three criteria are not matched, then that is wrong data, skip.
    }

    return E_DOING;
}

ERR_E Longpack::readLongpack(uint8_t *data, uint8_t len) {
    ERR_E result = E_DOING;
    for (uint8_t i = 0; i < len; i++) {
        result = readLongpack(data[i]);
        if (result != E_DOING)
            break;
    }
    return result;
}

void Longpack::sendLongpack(uint8_t *data, uint16_t len) {
    uint16_t dataLen = 0;
    dataLen = (data == NULL) ? 0 : len;

    PackHead headInfo;

    headInfo.magic1 = MAGIC_PART_1;
    headInfo.magic2 = MAGIC_PART_2;

    headInfo.lenHigh = dataLen >> 8 & 0xff;
    headInfo.lenLow = dataLen & 0xff;

    headInfo.version = 0x00;

    headInfo.lenCheck = headInfo.lenHigh ^ headInfo.lenLow;

    uint16_t checksum = CalcChecksum(data, len);
    headInfo.dataCheckHigh = checksum >> 8 & 0xff;
    headInfo.dataCheckLow = checksum & 0xff;

    // send head info
    uint8_t head[8];
    uint8_t * iter = (uint8_t *) &headInfo;
    for (int i = 0; i < 8; ++i) {
        head[i] = *iter;
        ++iter;
    }
    sendPack(head, 8);

    // send data field
    sendPack(data, len);
}

void Longpack::sendLongpack(uint16_t *data, uint16_t len) {
  sendLongpack((uint8_t*) data, len * 2);
}

void Longpack::cmd_clean() {
    memset(packData_, 0, sizeof(packData_));
}

void Longpack::sendPack(uint8_t *data, uint16_t len) {
    uint8_t tx_len = 0;
    uint8_t tx_data[8];

    for (uint16_t i = 0; i < len; i++) {
        tx_data[tx_len++] = data[i];
        if (tx_len >= 8) {
            can->sendMsgBuf(tx_id, 8, tx_data);
            tx_len = 0;
        }
    }

    if (tx_len)
        can->sendMsgBuf(tx_id, tx_len, tx_data);
}
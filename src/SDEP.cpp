#include "SDEP.h"

#include <string.h>

namespace FactUtilEmbedded
{

template <class TSPI>
void SDEP<TSPI>::sendPacket(uint16_t command, const uint8_t* buf, uint8_t count, uint8_t more_data)
{
    sdepMsgCommand_t msgCmd;

    msgCmd.header.msg_type      = SDEP_MSGTYPE_COMMAND;
    msgCmd.header.cmd_id_high = highByte(command);
    msgCmd.header.cmd_id_low  = lowByte(command);
    msgCmd.header.length      = count;
    msgCmd.header.more_data   = (count == SDEP_MAX_PACKETSIZE) ? more_data : 0;

    // Copy payload
    if ( buf != NULL && count > 0) memcpy(msgCmd.payload, buf, count);
}

}

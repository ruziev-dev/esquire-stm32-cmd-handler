#include "esquire_message_handler.h"

bool esquire_message_handler(esquire_Packet *message)
{
    switch (message->which_msg)
    {
    case esquire_Packet_root_fs_tree_tag:
        return true;

    case esquire_Packet_message_ok_tag:
        return true;

    case esquire_Packet_message_error_tag:
        return true;

    case esquire_Packet_ecosystem_cmd_tag:
        return esquire_cmd_handler(&message->msg.ecosystem_cmd);

    case esquire_Packet_ecu_spec_cmd_tag:
        return true;//esquire_cmd_handler(&message->msg.ecu_spec_cmd);
    default:
        return false;
    }

    return false;
}
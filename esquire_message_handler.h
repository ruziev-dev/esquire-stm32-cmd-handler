#ifndef __ESQUIRE_MESSAGE_HANDLER_H__
#define __ESQUIRE_MESSAGE_HANDLER_H__
#include "pb.h"
#include "Ecosystem.pb.h"

bool esquire_message_handler(esquire_Packet *message);
bool esquire_cmd_handler(esquire_EcosystemCommand *cmd);


#endif
#include "commandevent.h"
#include "eventvisitor.h"
#include "client.h"

CommandEvent::CommandEvent(::Command *cmd, CommandContext &&ctx, const std::shared_ptr<Client> &client) :
    mCommand(cmd),
    mContext(ctx),
    mClient(client)
{

}

void CommandEvent::execute()
{
    mCommand->execute(mContext, mClient->msgCtx());
}

IMPL_ACCEPT_EVENT_VISITOR(CommandEvent)

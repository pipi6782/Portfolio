#include "CAction.h"
#include "CAttachment.h"

TArray<ACAttachment*> UCAction::GetAttachments()
{
    return Attachments;
}

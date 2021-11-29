#include "EditorPch.h"
#include "EditorPostMaster.h"

namespace Editor
{
    void PostMaster::AddObserver(PostObserver* anObserver, CommandType aCommand)
    {
        myObservers[aCommand].push_back(anObserver);
    }

    void PostMaster::RemoveObserver(PostObserver* anObserver, CommandType aCommand)
    {
        myObservers[aCommand].Remove(anObserver);
    }

    void PostMaster::PostCommand(const Editor::PostCommand& aCommand)
    {
        for (auto obs : myObservers[aCommand.myType].AsVector())
        {
            obs->OnCommand(aCommand);
        }
    }
}

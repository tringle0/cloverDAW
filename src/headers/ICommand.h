#pragma once

class ICommand{
    virtual void execute();
    virtual void undo();
};
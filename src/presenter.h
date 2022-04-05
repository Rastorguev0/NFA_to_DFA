#pragma once
#include <ostream>

#include "fa.h"
#include "stream_io.h"

class FSAPresenter
{
public:
    static void Present(const FSA& fsa, std::ostream& os);
private:
};
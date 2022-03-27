#pragma once
#include <ostream>

#include "fa.h"
#include "stream_io.h"

class FAPresenter
{
public:
    static void Present(const FA& fa, std::ostream& os);
private:
};
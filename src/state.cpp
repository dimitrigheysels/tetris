#include <spdlog/spdlog.h>

#include "state.h"
#include "game.h"

State::State(Game &game) : game_(game)
{
}

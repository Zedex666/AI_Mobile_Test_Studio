#include "llm_provider.h"

namespace core {
namespace llm {

LLMProvider::LLMProvider(const LLMConfig &config, QObject *parent)
    : QObject(parent)
    , m_config(config)
{
}

LLMProvider::~LLMProvider()
{
}

} // namespace llm
} // namespace core

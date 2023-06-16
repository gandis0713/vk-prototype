#include "sample.h"

namespace vkt
{

Sample::Sample(const SampleDescriptor& descriptor)
    : Window(descriptor.windowDescriptor)
    , m_path(descriptor.path)
{
}

} // namespace vkt
#include "controller/filters/compositefilter.h"

CompositeFilter::CompositeFilter(LogicalOperator op)
    : m_operator(op) {
}

bool CompositeFilter::apply(const Artwork& artwork) const {
    if (m_filters.isEmpty()) {
        return true;
    }

    if (m_operator == AND) {
        for (const auto& filter : m_filters) {
            if (!filter->apply(artwork)) {
                return false;
            }
        }
        return true;
    } else {
        for (const auto& filter : m_filters) {
            if (filter->apply(artwork)) {
                return true;
            }
        }
        return false;
    }
}

void CompositeFilter::addFilter(std::shared_ptr<Filter> filter) {
    m_filters.append(filter);
}

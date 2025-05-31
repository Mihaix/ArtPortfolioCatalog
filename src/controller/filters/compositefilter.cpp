#include "controller/filters/compositefilter.h"

CompositeFilter::CompositeFilter(LogicalOperator op)
    : m_operator(op) {
}

bool CompositeFilter::apply(const Artwork& artwork) const {
    if (m_filters.isEmpty()) {
        return true; // No filters means no restrictions
    }

    if (m_operator == AND) {
        // All filters must match (AND logic)
        for (const auto& filter : m_filters) {
            if (!filter->apply(artwork)) {
                return false;
            }
        }
        return true;
    } else {
        // At least one filter must match (OR logic)
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

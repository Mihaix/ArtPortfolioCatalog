#ifndef COMPOSITEFILTER_H
#define COMPOSITEFILTER_H

#include "controller/filters/filter.h"
#include <QVector>
#include <memory>

// Composite Pattern combined with Strategy Pattern
class CompositeFilter : public Filter {
public:
    enum LogicalOperator {
        AND,
        OR
    };

    explicit CompositeFilter(LogicalOperator op = AND);

    bool apply(const Artwork& artwork) const override;

    void addFilter(std::shared_ptr<Filter> filter);

private:
    QVector<std::shared_ptr<Filter>> m_filters;
    LogicalOperator m_operator;
};

#endif // COMPOSITEFILTER_H

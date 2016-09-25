#pragma once

#include <cstddef>
#include <stdexcept>
#include <vector>


template <class T, class Combiner, class Modifier>
class Sequence {
public:
    Sequence(size_t size, const T& init_value)
      : sequence_(size, init_value)
    { }

    Sequence(size_t size, const T& init_value, Combiner combiner, Modifier modifier)
      : combiner_(combiner)
      , modifier_(modifier)
      , sequence_(size, init_value)
    { }

    size_t Size() const
    {
        return sequence_.size();
    }

    void Set(size_t range_begin, size_t range_end, const T& value)
    {
        if (range_begin > range_end) {
            throw std::invalid_argument("Sequence.Set: range_begin > range_end");
        }
        for (size_t index = range_begin; index < range_end; ++index) {
            sequence_[index] = value;
        }
    }

    T Combine(size_t range_begin, size_t range_end) const
    {
        if (range_begin >= range_end) {
            throw std::invalid_argument("Sequence.Combine: range_begin >= range_end");
        }
        T result = sequence_.at(range_begin);
        for (size_t index = range_begin + 1; index < range_end; ++index) {
            result = combiner_(result, sequence_.at(index));
        }
        return result;
    }

    void Modify(size_t range_begin, size_t range_end, const T& modifier_left_argument)
    {
        if (range_begin > range_end) {
            throw std::invalid_argument("Sequence.Modify: range_begin > range_end");
        }
        for (size_t index = range_begin; index < range_end; ++index) {
            sequence_[index] = modifier_(modifier_left_argument, sequence_[index]);
        }
    }

private:
    Combiner combiner_;
    Modifier modifier_;
    std::vector<T> sequence_;
};

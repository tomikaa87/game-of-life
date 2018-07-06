#ifndef BOOLMATRIX_H
#define BOOLMATRIX_H

#include <vector>

class IBoolMatrix
{
public:
    virtual bool valueAt(uint16_t col, uint16_t row) const = 0;
    virtual void setValueAt(const uint16_t col, const uint16_t row, const bool value) = 0;

    virtual uint16_t rows() const = 0;
    virtual uint16_t columns() const = 0;
};

template <uint16_t N, uint16_t M>
class BoolMatrix: public IBoolMatrix
{
public:
    BoolMatrix()
    {
        m_data.resize(M);
        for (auto&& row : m_data)
        {
            row.resize(N);
        }
    }

    bool valueAt(uint16_t col, uint16_t row) const override
    {
        if (col < 0 || col >= columns() || row < 0 || row >= rows())
            return false;

        return m_data[row][col];
    }

    void setValueAt(const uint16_t col, const uint16_t row, const bool value) override
    {
        m_data[row][col] = value;
    }

    uint16_t rows() const override
    {
        return M;
    }

    uint16_t columns() const override
    {
        return N;
    }

private:
    std::vector<std::vector<bool>> m_data;
};

#endif // BOOLMATRIX_H

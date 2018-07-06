#include "engine.h"

#include <QTimer>

Engine::Engine(QObject *parent)
    : QObject(parent)
    , m_timer(new QTimer(this))
{
    m_timer->setInterval(250);

    connect(m_timer, &QTimer::timeout, this, &Engine::timerTimeout);
}

uint32_t Engine::generation() const
{
    return m_generation;
}

const IBoolMatrix &Engine::cellMatrix() const
{
    return m_matrix;
}

void Engine::setCellMatrix(const Engine::MatrixType &matrix)
{
    m_generation = 0;
    m_matrix = matrix;
    emit updated();
}

void Engine::toggleCell(uint16_t col, uint16_t row)
{
    m_matrix.setValueAt(col, row, !m_matrix.valueAt(col, row));
    emit updated();
}

void Engine::start()
{
    m_timer->start();
    emit updated();
}

void Engine::stop()
{
    m_timer->stop();
    emit updated();
}

bool Engine::isRunning() const
{
    return m_timer->isActive();
}

void Engine::randomize()
{
    stop();

    for (int col = 0; col < m_matrix.columns(); ++col)
    {
        for (int row = 0; row < m_matrix.rows(); ++row)
        {
            m_matrix.setValueAt(col, row, qrand() % 2 > 0);
        }
    }

    m_generation = 0;

    emit updated();
}

void Engine::clear()
{
    m_matrix = {};
    m_generation = 0;
    emit updated();
}

void Engine::step()
{
    stop();
    advance();
    emit updated();
}

void Engine::timerTimeout()
{
    advance();
    emit updated();
}

void Engine::advance()
{
    decltype(m_matrix) newMatrix;

    for (int col = 0; col < m_matrix.columns(); ++col)
    {
        for (int row = 0; row < m_matrix.rows(); ++row)
        {
            int nc = neighborCount(col, row);
            bool alive = m_matrix.valueAt(col, row);

            if (alive)
            {
                if (nc >= 2 && nc <= 3)
                    newMatrix.setValueAt(col, row, true);
                else
                    newMatrix.setValueAt(col, row, false);
            }
            else
            {
                if (nc == 3)
                {
                    newMatrix.setValueAt(col, row, true);
                }
            }
        }
    }

    m_matrix = newMatrix;

    ++m_generation;
}

int Engine::neighborCount(uint16_t col, uint16_t row) const
{
    int count = 0;

    // top left
    if (m_matrix.valueAt(col - 1, row - 1))
        ++count;

    // top center
    if (m_matrix.valueAt(col, row - 1))
        ++count;

    // top right
    if (m_matrix.valueAt(col + 1, row - 1))
        ++count;

    // left
    if (m_matrix.valueAt(col - 1, row))
        ++count;

    // right
    if (m_matrix.valueAt(col + 1, row))
        ++count;

    // bottom left
    if (m_matrix.valueAt(col - 1, row + 1))
        ++count;

    // bottom
    if (m_matrix.valueAt(col, row + 1))
        ++count;

    // bottom right
    if (m_matrix.valueAt(col + 1, row + 1))
        ++count;

    return count;
}

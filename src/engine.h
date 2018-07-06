#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>

#include "boolmatrix.h"

class QTimer;

class Engine : public QObject
{
    Q_OBJECT

public:
    explicit Engine(QObject *parent = nullptr);

    using MatrixType = BoolMatrix<100, 40>;

    uint32_t generation() const;

    const IBoolMatrix& cellMatrix() const;
    void setCellMatrix(const MatrixType& matrix);

    void toggleCell(uint16_t col, uint16_t row);

    void start();
    void stop();
    bool isRunning() const;

    void randomize();
    void clear();

    void step();

signals:
    void updated();

private:
    uint32_t m_generation = 0;
    QTimer* m_timer = nullptr;
    MatrixType m_matrix;

    void timerTimeout();
    void advance();

    int neighborCount(uint16_t col, uint16_t row) const;
};

#endif // ENGINE_H

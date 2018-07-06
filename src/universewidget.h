#ifndef UNIVERSEWIDGET_H
#define UNIVERSEWIDGET_H

#include <QImage>
#include <QObject>
#include <QWidget>

#include "boolmatrix.h"

class UniverseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UniverseWidget(QWidget *parent = nullptr);

    void setRenderScaleFactor(const uint8_t factor);

    void setCellMatrix(const IBoolMatrix& m);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

signals:
    void cellToggled(uint16_t col, uint16_t row);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    QImage m_image;
    uint8_t m_renderScaleFactor = 4;
    QRect m_lastImageRect;
};

#endif // UNIVERSEWIDGET_H

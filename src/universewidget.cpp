#include "universewidget.h"

#include <QDebug>
#include <QLoggingCategory>
#include <QMouseEvent>
#include <QPainter>

Q_LOGGING_CATEGORY(UniverseWidgetLog, "UniverseWidget")

UniverseWidget::UniverseWidget(QWidget *parent)
    : QWidget(parent)
{

}

void UniverseWidget::setRenderScaleFactor(const uint8_t factor)
{
    m_renderScaleFactor = factor;
    update();
}

void UniverseWidget::setCellMatrix(const IBoolMatrix &m)
{
    QSize matrixSize{ m.columns(), m.rows() };

//    qCDebug(UniverseWidgetLog) << "setCellMatrix size:" << matrixSize;

    if (m_image.size() != matrixSize)
    {
        m_image = QImage{ matrixSize, QImage::Format_Mono };
        m_image.fill(Qt::white);
    }

    for (uint32_t y = 0; y < m.rows(); ++y)
    {
        for (uint32_t x = 0; x < m.columns(); ++x)
        {
//            qCDebug(UniverseWidgetLog) << "setting pixel" << x << ":" << y << "to" << m.valueAt(x, y);

            m_image.setPixel(x, y, m.valueAt(x, y) ? 0 : 1);
        }
    }

    update();
}

QSize UniverseWidget::minimumSizeHint() const
{
    qCDebug(UniverseWidgetLog) << "minimumSizeHint():" << sizeHint();
    return sizeHint();
}

QSize UniverseWidget::sizeHint() const
{
    qCDebug(UniverseWidgetLog) << "sizeHint():" << m_image.size() * m_renderScaleFactor;
    return m_image.size() * m_renderScaleFactor;
}

void UniverseWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    if (m_image.isNull())
        return;

    QPainter p{ this };

    auto&& scaledImage = m_image.scaled(size(), Qt::KeepAspectRatio);

    QRect imageRect;
    imageRect.setSize(scaledImage.size());
    imageRect.moveCenter(rect().center());

    m_lastImageRect = imageRect;
    m_lastScaledImage = scaledImage;

    p.drawImage(imageRect, scaledImage);
}

void UniverseWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);

    auto&& p = event->pos() - m_lastImageRect.topLeft();

    double currentScaleFactorX = static_cast<double>(m_lastImageRect.width()) / m_image.width();
    double currentScaleFactorY = static_cast<double>(m_lastImageRect.height()) / m_image.height();

    qCDebug(UniverseWidgetLog) << "mousePressEvent, scale factors:" << currentScaleFactorX << currentScaleFactorY;

    emit cellToggled(p.x() / currentScaleFactorX, p.y() / currentScaleFactorY);
}

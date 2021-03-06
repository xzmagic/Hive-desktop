#ifndef GUIBADGEICON_H
#define GUIBADGEICON_H

#include <QLabel>
#include <QPainter>


class BadgeIcon : public QWidget
{
  Q_OBJECT
public:
  explicit BadgeIcon(const int &dia,QWidget *parent = 0);

  void setNumber(const int &number);

protected:
  void paintEvent(QPaintEvent*);

private:
  int number;
  int height;
  QRectF rectangle;
  QColor color = QColor("#D31111");
};

#endif // GUIBADGEICON_H

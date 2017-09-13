
#include	"ItemDelegate.hh"

Plazza::ItemDelegate::ItemDelegate(Plazza::Gui *mainWindow, int val, int nbCook) : QItemDelegate(mainWindow)
{
  this->value = val;
  this->nbCook = nbCook;
}

void	Plazza::ItemDelegate::setValue(int value)
{
  this->value = value;
}

void Plazza::ItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
		  const QModelIndex &index) const
{
  QStyleOptionProgressBar progressBarOption;

  (void)index;
  progressBarOption.state = QStyle::State_Enabled;
  progressBarOption.direction = QApplication::layoutDirection();
  progressBarOption.fontMetrics = QApplication::fontMetrics();
  progressBarOption.minimum = 0;
  progressBarOption.maximum = this->nbCook;
  progressBarOption.textAlignment = Qt::AlignCenter;
  progressBarOption.textVisible = true;
  progressBarOption.rect = option.rect;

  progressBarOption.progress = this->value;
  progressBarOption.text = QString().sprintf("%d/%d", progressBarOption.progress, this->nbCook);
  QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
}

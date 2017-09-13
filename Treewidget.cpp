#include	"Treewidget.hh"

TreeWidget::TreeWidget(QWidget *parent) : QTreeWidget(parent)
{
  QTreeWidgetItem *headerItem;

  this->setGeometry(QRect(40, 280, 701, 211));
  this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  this->header()->setCascadingSectionResizes(false);
  this->viewport()->setProperty("cursor", QVariant(QCursor(Qt::PointingHandCursor)));
  this->setMouseTracking(true);

  headerItem = this->headerItem();
  headerItem->setText(0, "Cuisines");
  headerItem->setText(1, "En cours");
  headerItem->setText(2, "Total traitees");
  headerItem->setText(3, "En attente");
  headerItem->setText(4, "");
  for (int i = 0; i < 4; i++)
    headerItem->setTextAlignment(i, Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);

  this->resizeColumnToContents(0);
  this->resizeColumnToContents(1);
  this->resizeColumnToContents(2);
  this->resizeColumnToContents(3);
  this->header()->setMovable(false);
  this->header()->setClickable(false);
  this->header()->setHighlightSections(true);
  this->setSelectionBehavior(QAbstractItemView::SelectRows);
}

TreeWidget::~TreeWidget()
{}

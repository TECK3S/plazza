#ifndef		__ITEMDELEGATE_HH__
#define		__ITEMDELEGATE_HH__

#include	<Qt/QtGui>
#include	"Gui.hh"

namespace Plazza
{
  class Gui;

  class ItemDelegate : public QItemDelegate
  {
    Q_OBJECT
    public:
    ItemDelegate(Plazza::Gui *mainWindow, int, int);
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
	       const QModelIndex &index ) const;
    void	setValue(int value);

    private:
     int value;
    int nbCook;
  };
}

#endif


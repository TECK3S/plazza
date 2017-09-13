#ifndef		__GUI_HH__
#define		__GUI_HH__

#include	<Qt/QtGui>
#include	<string>
#include	<list>
#include	"Treewidget.hh"
#include	"KitchenState.hh"
#include        "ItemDelegate.hh"



namespace Plazza
{
  class ItemDelegate;
  class	Gui : public QMainWindow
  {
    Q_OBJECT
    public:
    Gui(int nbCook);
    ~Gui();

    void	display(KitchenState  &);
    std::string const& getInput();
    void        closeEvent(QCloseEvent *e);
    bool	is_opened() const;
    void        destroy_row();
    void	create_row(KitchenState const &);

  public slots:
    void	historique();
    void        getLineEditText();
    void	express_cmd();
    void	express_valid();

  private:
    std::string   inputText, copy;
    QLineEdit     *lineEdit;
    bool	  is_open;
    TreeWidget    *treewidget;
    QLabel        *label_7, *label_8, *label_9, *label_10;
    std::list<ItemDelegate *> v_ItemDelegate;
    QRadioButton *radioButton[9];
    QSpinBox *spinBox;
    QWidget *widget;
    int		nbCook;
  };
}

#endif

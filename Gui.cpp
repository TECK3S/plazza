#include	"Gui.hh"
#include	"Treewidget.hh"
#include	"ItemDelegate.hh"
#include        "KitchenState.hh"

Plazza::Gui::Gui(int nbCook) : QMainWindow()
{
  QMenu		*actions;
  QAction       *historique, *aquit;
  QFont		font;
  QPushButton	*pushButton, *pushButton_2;
  QLabel	*label, *label_2, *label_3,
    *label_4, *label_5, *label_6, *label_11, *label_12;

  this->nbCook = nbCook;
  font.setFamily(QString::fromUtf8("Century Schoolbook L"));
  font.setPointSize(9);
  font.setBold(true);
  font.setWeight(75);
  this->setFont(font);

  this->setPalette(QColor(Qt::white));

  label_11 = new QLabel(this);
  label_12 = new QLabel(this);
  label = new QLabel(this);
  label_2 = new QLabel(this);
  label_3 = new QLabel(this);
  label_4 = new QLabel(this);
  label_5 = new QLabel(this);
  label_6 = new QLabel(this);
  label_7 = new QLabel(this);
  label_8 = new QLabel(this);
  label_9 = new QLabel(this);
  label_10 = new QLabel(this);

  label_11->setGeometry(QRect(0, 0, 821, 525));
  label_11->setPixmap(QPixmap("includes/pizza-hut-pizza.jpg"));

  label_12->setGeometry(QRect(650, -10, 151, 171));
  label_12->setPixmap(QPixmap("includes/pizza-hut.jpg"));

  label->setGeometry(QRect(340, 110, 171, 18));
  label_2->setGeometry(QRect(610, 200, 141, 21));
  label_3->setGeometry(QRect(540, 180, 211, 18));
  label_4->setGeometry(QRect(610, 220, 181, 18));
  label_5->setGeometry(QRect(40, 260, 81, 18));
  label_6->setGeometry(QRect(610, 240, 141, 18));
  this->label_7->setGeometry(QRect(40, 540, 141, 18));
  this->label_8->setGeometry(QRect(210, 540, 131, 18));
  this->label_9->setGeometry(QRect(380, 540, 151, 18));
  this->label_10->setGeometry(QRect(660, 540, 81, 18));


  label_5->setFrameShape(QFrame::StyledPanel);
  this->label_7->setFrameShape(QFrame::StyledPanel);
  this->label_8->setFrameShape(QFrame::StyledPanel);
  this->label_9->setFrameShape(QFrame::StyledPanel);
  this->label_10->setFrameShape(QFrame::StyledPanel);

  label->setText("Tapez votre commande ici");
  label_2->setText("Type := [a..zA..z]+");
  label_3->setText("Syntaxe :     Type  Taille  Nombre");
  label_4->setText("Taille := S | M | L | XL |XXL");
  label_5->setText("Statistiques");
  label_6->setText("Nombre := x[1..9] [0..9]*");
  this->label_7->setText("Total en attente =  0");
  this->label_8->setText("Total en cours =  0");
  this->label_9->setText("Total en instance =  0");
  this->label_10->setText("Total =  0");

  this->setFixedSize(800, 600);
  this->setWindowTitle("PLAZZA");
  this->move((QApplication::desktop()->width() - this->width()) / 2, (QApplication::desktop()->height() - this->height()) /2);
  this->setWindowIcon(QIcon("qt.png"));

  actions = menuBar()->addMenu("Actions");
  historique = new QAction("Historique des commandes", this);
  aquit = new QAction("Quitter", this);
  actions->addAction(menuBar()->addSeparator());
  actions->addAction(historique);
  actions->addAction(menuBar()->addSeparator());
  actions->addAction(aquit);

  this->treewidget = new TreeWidget(this);

  this->lineEdit = new QLineEdit(this);
  this->lineEdit->setGeometry(QRect(230, 140, 381, 28));
  this->lineEdit->setPalette(QPalette(Qt::white));
  this->lineEdit->setMaxLength(256);
  this->lineEdit->setText("regina XXL x2");

  pushButton = new QPushButton(this);
  pushButton->setGeometry(QRect(350, 190, 151, 28));
  pushButton->setText("Lancer la commande");

  pushButton_2 = new QPushButton(this);
  pushButton_2->setGeometry(QRect(360, 230, 131, 28));
  pushButton_2->setText("Commande express");

  this->connect(pushButton_2, SIGNAL(clicked()), this, SLOT(express_cmd()));
  this->connect(historique, SIGNAL(triggered()), this, SLOT(historique()));
  this->connect(aquit, SIGNAL(triggered()), this, SLOT(close()));
  this->connect(this->lineEdit, SIGNAL(returnPressed()), this, SLOT(getLineEditText()));
  this->connect(pushButton, SIGNAL(clicked()), this, SLOT(getLineEditText()));
  this->is_open = true;

  this->show();
}

Plazza::Gui::~Gui()
{

}

void        Plazza::Gui::express_cmd()
{
  QLabel *label, *label_2, *label_3;
  QWidget *verticalLayoutWidget, *verticalLayoutWidget_2;
  QVBoxLayout *verticalLayout, *verticalLayout_2;
  QPushButton *pushButton;
  QFont	font;

  this->widget = new QWidget;
  this->widget->setFixedSize(473, 256);
  this->widget->move((this->x() - widget->x()) + 150, (this->y() - widget->y()) + 150);
  this->widget->setWindowModality(Qt::ApplicationModal);

  font.setFamily("Century Schoolbook L");
  font.setPointSize(9);
  font.setBold(true);
  font.setWeight(75);
  widget->setFont(font);

  label = new QLabel(widget);
  label->setGeometry(QRect(40, 20, 91, 18));
  label->setFrameShape(QFrame::StyledPanel);
  label->setFrameShadow(QFrame::Plain);

  label_2 = new QLabel(widget);
  label_2->setGeometry(QRect(270, 20, 51, 18));
  label_2->setFrameShape(QFrame::StyledPanel);
  label_2->setFrameShadow(QFrame::Plain);

  verticalLayoutWidget = new QWidget(widget);
  verticalLayoutWidget->setGeometry(QRect(40, 60, 181, 131));
  verticalLayout = new QVBoxLayout(verticalLayoutWidget);
  verticalLayout->setContentsMargins(0, 0, 0, 0);

  for (int i = 0; i < 4; i++)
    this->radioButton[i] = new QRadioButton(verticalLayoutWidget);
  for (int i = 0; i < 4; i++)
    verticalLayout->addWidget(radioButton[i]);

  verticalLayoutWidget_2 = new QWidget(widget);
  verticalLayoutWidget_2->setGeometry(QRect(270, 60, 81, 141));
  verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
  verticalLayout_2->setContentsMargins(0, 0, 0, 0);

  for (int i = 4; i < 9; i++)
    this->radioButton[i] = new QRadioButton(verticalLayoutWidget_2);
  for (int i = 4; i < 9; i++)
    verticalLayout_2->addWidget(radioButton[i]);

  this->radioButton[0]->setChecked(true);
  this->radioButton[4]->setChecked(true);

  this->spinBox = new QSpinBox(widget);
  this->spinBox->setGeometry(QRect(410, 60, 51, 28));
  this->spinBox->setMinimum(1);

  label_3 = new QLabel(widget);
  label_3->setGeometry(QRect(400, 20, 62, 18));
  label_3->setFrameShape(QFrame::StyledPanel);

  pushButton = new QPushButton(widget);
  pushButton->setGeometry(QRect(390, 210, 61, 28));

  label->setText("Type de pizza");
  label_2->setText("Taille");

  this->radioButton[0]->setText("Regina");
  this->radioButton[1]->setText("Margarita");
  this->radioButton[2]->setText("Fantasia");
  this->radioButton[3]->setText("Americaine");
  this->radioButton[4]->setText("S");
  this->radioButton[5]->setText("M");
  this->radioButton[6] ->setText("L");
  this->radioButton[7]->setText("XL");
  this->radioButton[8]->setText("XXL");

  label_3->setText("Quantite");
  pushButton->setText("Lancer");

  this->connect(pushButton, SIGNAL(clicked()), this, SLOT(express_valid()));
  widget->show();
}

void  Plazza::Gui::express_valid()
{
  for (int i = 0; i < 9; i++)
    {
      if (this->radioButton[i]->isChecked())
	{
	  this->inputText.append(this->radioButton[i]->text().toStdString());
	  this->inputText.append(" ");
	}
    }
  this->inputText.append("x");
  this->inputText.append(QString::number(this->spinBox->value()).toStdString());
  this->widget->close();
  this->widget->deleteLater();
  if (!this->lineEdit->text().isEmpty())
    this->lineEdit->clear();
}

void  Plazza::Gui::historique()
{

}

void  Plazza::Gui::display(Plazza::KitchenState &state)
{
  std::list<int>::const_iterator it, it1, it2;
  int	i = 0;

  this->destroy_row();
  this->create_row(state);

  this->label_7->setText("Total en attente =  " + QString::number(state.waitingTotal));
  this->label_8->setText("Total en cours =  " + QString::number(state.ongoingTotal));
  this->label_9->setText("Total en instance =  " + QString::number(state.pendingCommands));
  this->label_10->setText("Total =  " + QString::number(state.totalTotal));

  for (it = state.waitingKitchenCmds.begin(), it1 = state.ongoingKitchenCmds.begin(), it2 = state.totalKitchenCmds.begin();
       it != state.waitingKitchenCmds.end(); ++it, ++it1, ++it2)
    {
      this->treewidget->topLevelItem(i)->setText(1, QString::number((*it)));
      this->treewidget->topLevelItem(i)->setText(3, QString::number((*it1)));
      this->treewidget->topLevelItem(i)->setText(2, QString::number((*it2)));
      i++;
    }

  if (QCoreApplication::hasPendingEvents())
    QCoreApplication::processEvents();
}

void Plazza::Gui::getLineEditText()
{
  QString tmp;

  tmp = this->lineEdit->text().trimmed();
  if (!tmp.isEmpty())
    {
      this->inputText = tmp.toStdString();
      this->lineEdit->clear();
    }
  else
    QMessageBox::critical(this, "Erreur", "Tapez votre commande dans le champ specifie.");
}

std::string const& Plazza::Gui::getInput()
{
  copy = this->inputText;

  if (!copy.empty())
    this->inputText.clear();
  return (copy);
}

void  Plazza::Gui::closeEvent(QCloseEvent *)
{
  this->is_open = false;
}

bool  Plazza::Gui::is_opened() const
{
  return (this->is_open);
}

void  Plazza::Gui::destroy_row()
{
  std::list<ItemDelegate *>::iterator it;

  if (this->treewidget->topLevelItemCount() != 0)
    {
      this->treewidget->clear();
      for (it = this->v_ItemDelegate.begin(); it != this->v_ItemDelegate.end(); ++it)
	delete (*it);
      this->v_ItemDelegate.clear();
    }
}

void  Plazza::Gui::create_row(KitchenState const &state)
{
  std::list<int>::const_iterator it, it1;
  int	j = 1;

  for (it = state.waitingKitchenCmds.begin(), it1 = state.ongoingKitchenCmds.begin();
       it != state.waitingKitchenCmds.end(); ++it, ++it1)
    {
      QTreeWidgetItem *item = new QTreeWidgetItem;
      Plazza::ItemDelegate    *it3 = new ItemDelegate(this, (*it), this->nbCook);
      Plazza::ItemDelegate    *it4 = new ItemDelegate(this, (*it1), this->nbCook);

      item->setText(0, QString::number(j));
      item->setTextAlignment(0, Qt::AlignHCenter);
      item->setTextAlignment(2, Qt::AlignHCenter);
      this->treewidget->addTopLevelItem(item);
      this->treewidget->setItemDelegateForColumn(1, it3);
      this->treewidget->setItemDelegateForColumn(3, it4);
      this->v_ItemDelegate.push_back(it3);
      this->v_ItemDelegate.push_back(it4);
      j++;
    }
}

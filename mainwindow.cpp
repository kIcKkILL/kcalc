#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QMessageBox>
#include "cCalcore.h" //maybe not proper here,better with an interface header
#include <exception>
#include <QClipboard>

bool isOperator(QChar c) {
  switch (c.toAscii()) {
    case '+': case '-': case '*': case '/': case '%': case '!': case '^':
      return true;
    default:
      return false;
  }
}

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->statusBar->showMessage("Welcome!");
  connect(ui->actionSwitch_mode,SIGNAL(triggered()),this,SLOT(changeWorkMode()));
  // setup objects in mainwindow.ui

  work_mode = FINAL;
  result_mode = DEC;
  exp_buffer_ = new QString;

  for(int i=1; i!=10; ++i) {
    num_button[i] = new QPushButton(QString(i+'0'),this);
    num_button[i]->setFocusPolicy(Qt::NoFocus);
    num_button[i]->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    ui->button_layout->addWidget(num_button[i],3-(i-1)/3,(i-1)%3);
    // traditional calculator's layout
    connect(num_button[i],SIGNAL(clicked()),this,SLOT(num_button_clicked()));
  }
  num_button[0] = new QPushButton(QString('0'),this);
  num_button[0]->setFocusPolicy(Qt::NoFocus);
  num_button[0]->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
  connect(num_button[0],SIGNAL(clicked()),this,SLOT(num_button_clicked()));
  ui->button_layout->addWidget(num_button[0],4,0);
  // all digital button initialized

  point_button = new QPushButton(".",this);
  point_button->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
  point_button->setFocusPolicy(Qt::NoFocus);
  connect(point_button,SIGNAL(clicked()),this,SLOT(num_button_clicked()));
  ui->button_layout->addWidget(point_button,4,1);
  // point_button, treated as num_button

  equal_button = new QPushButton("=",this);
  equal_button->setFocusPolicy(Qt::NoFocus);
  equal_button->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
  connect(equal_button,SIGNAL(clicked()),this,SLOT(equal_button_clicked()));
  ui->button_layout->addWidget(equal_button,4,2);
  // equal_button

  plus_button = new QPushButton("+",this);
  plus_button->setFocusPolicy(Qt::NoFocus);
  plus_button->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
  connect(plus_button,SIGNAL(clicked()),this,SLOT(operator_button_clicked()));
  ui->button_layout->addWidget(plus_button,1,3);
  // plus_button

  minus_button = new QPushButton("-",this);
  minus_button->setFocusPolicy(Qt::NoFocus);
  minus_button->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
  connect(minus_button,SIGNAL(clicked()),this,SLOT(operator_button_clicked()));
  ui->button_layout->addWidget(minus_button,2,3);
  // minus_button

  multiply_button = new QPushButton("*",this);
  multiply_button->setFocusPolicy(Qt::NoFocus);
  multiply_button->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
  connect(multiply_button,SIGNAL(clicked()),this,SLOT(operator_button_clicked()));
  ui->button_layout->addWidget(multiply_button,3,3);
  // multiply_button

  division_button = new QPushButton("/",this);
  division_button->setFocusPolicy(Qt::NoFocus);
  division_button->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
  connect(division_button,SIGNAL(clicked()),this,SLOT(operator_button_clicked()));
  ui->button_layout->addWidget(division_button,4,3);
  // division_button

  clear_button = new QPushButton("C",this);
  clear_button->setFocusPolicy(Qt::NoFocus);
  clear_button->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
  connect(clear_button,SIGNAL(clicked()),this,SLOT(clear_input()));
  ui->button_layout->addWidget(clear_button,0,4);
  // clear_button

  backspace_button = new QPushButton("DEL",this);
  backspace_button->setFocusPolicy(Qt::NoFocus);
  backspace_button->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
  connect(backspace_button,SIGNAL(clicked()),this,SLOT(backspace()));
  ui->button_layout->addWidget(backspace_button,0,3);
  // backspace_button

  mod_button = new QPushButton("MOD",this);
  mod_button->setFocusPolicy(Qt::NoFocus);
  mod_button->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
  connect(mod_button,SIGNAL(clicked()),this,SLOT(operator_button_clicked()));
  ui->button_layout->addWidget(mod_button,1,4);
  // mod_button

  power_button = new QPushButton("^",this);
  power_button->setFocusPolicy(Qt::NoFocus);
  power_button->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
  connect(power_button,SIGNAL(clicked()),this,SLOT(operator_button_clicked()));
  ui->button_layout->addWidget(power_button,2,4);
  // power_button

  factorial_button = new QPushButton("!",this);
  factorial_button->setFocusPolicy(Qt::NoFocus);
  factorial_button->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
  connect(factorial_button,SIGNAL(clicked()),this,SLOT(operator_button_clicked()));
  ui->button_layout->addWidget(factorial_button,3,4);
  // factorial_button

  pi_button = new QPushButton("PI",this);
  pi_button->setFocusPolicy(Qt::NoFocus);
  pi_button->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
  ui->button_layout->addWidget(pi_button,0,0);
  connect(pi_button,SIGNAL(clicked()),this,SLOT(num_button_clicked()));
  // pi_button, treated as num_button

  left_button = new QPushButton("(",this);
  left_button->setFocusPolicy(Qt::NoFocus);
  left_button->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
  connect(left_button,SIGNAL(clicked()),this,SLOT(left_button_clicked()));
  ui->button_layout->addWidget(left_button,0,1);
  // left_button

  right_button = new QPushButton(")",this);
  right_button->setFocusPolicy(Qt::NoFocus);
  right_button->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
  connect(right_button,SIGNAL(clicked()),this,SLOT(right_button_clicked()));
  ui->button_layout->addWidget(right_button,0,2);
  // right_button

  foo_button = new QPushButton("foo",this);
  foo_button->setFocusPolicy(Qt::NoFocus);
  foo_button->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
  connect(foo_button,SIGNAL(clicked()),this,SLOT(wtf()));
  ui->button_layout->addWidget(foo_button,4,4);
  // foo_button
}

void MainWindow::num_button_clicked() {
  QString num_string = this->sender()->property("text").toString();
  if (num_string=="PI") {
    if ( exp_status_.dot_used )
      return;
    num_string = QString::fromStdString(kPi_s);
    exp_status_.dot_used = true;
    exp_status_.post_dot_number = true;
  }
  // get number
  addNum(num_string);
}

void MainWindow::operator_button_clicked() {
  QString operation_string = this->sender()->property("text").toString();
  if (operation_string == "MOD")
    operation_string = "%";
  addOperator(operation_string);
}

void MainWindow::equal_button_clicked() {
  if (exp_buffer_->isEmpty())
    return;
  try {
    *exp_buffer_ += "=";
    std::string tmp = exp_buffer_->toStdString();
    cCore calculator(tmp);
    double temp_result = calculator.calculate();
    emit(clear_input());
    ui->input_box->setText(QString::fromStdString(tmp));
    ui->result_display->display(temp_result);
    result_buffer_ = temp_result;
    *exp_buffer_ = QString::fromStdString(doubleToString(result_buffer_));
  } catch(const std::exception &e) {
    emit(clear_input());
    ui->statusBar->showMessage(e.what());
  }
}

void MainWindow::clear_input() {
  exp_buffer_->clear();
  result_buffer_=0.0;
  exp_status_.clear();
  ui->input_box->setText("");
  ui->result_display->display(0.0);
  showWorkMode();
}

void MainWindow::backspace() {
  if (exp_buffer_->size() > 0) {
    QChar last = *(exp_buffer_->end()-1);
    if(isOperator(last))
      exp_status_.operator_used = false;
    else
      if(last == '.') {
        exp_status_.dot_used = false;
        exp_status_.post_dot_number = false;
      }

    if (exp_buffer_->size() > 1) {
      if(isOperator(*(exp_buffer_->end()-2)))
         exp_status_.operator_used = true;
      if(*(exp_buffer_->end()-2) == '.')
        exp_status_.post_dot_number = false;
      if(*(exp_buffer_->end()-2)=='!')
        exp_status_.operator_used = false;
    }
    // change expression status accordingly

    exp_buffer_->remove(exp_buffer_->size()-1,1);
  }
  ui->input_box->setText(*exp_buffer_);
}

void MainWindow::changeWorkMode() {
  work_mode = (work_mode == INSTANT? FINAL : INSTANT);
  emit(clear_input());
  // to prevent any potential bug
}

void MainWindow::left_button_clicked() {
  if(work_mode == INSTANT) {
    ui->statusBar->showMessage("Not allowed in INSTANT mode");
    return;
  }

  bool available =false;
  if (exp_buffer_->isEmpty() || exp_status_.operator_used)
    available = true;
  if (!exp_buffer_->isEmpty() && *(exp_buffer_->end()-1)=='(')
    available = true;

  if (available) {
    exp_buffer_->push_back('(');
    ui->input_box->setText(*exp_buffer_);

    exp_status_.dot_used = false;
    exp_status_.post_dot_number = false;
    exp_status_.operator_used = false;
  }

}

void MainWindow::right_button_clicked() {
  if(work_mode == INSTANT) {
    ui->statusBar->showMessage("Not allowed in INSTANT mode");
    return;
  }

  exp_buffer_->push_back(')');
  ui->input_box->setText(*exp_buffer_);

  exp_status_.dot_used = false;
  exp_status_.post_dot_number = false;
  exp_status_.operator_used = false;
  // to do: maybe nothing to do here
}

void MainWindow::wtf() {
  QMessageBox msgBox;
  msgBox.setText("bar");
  msgBox.exec();
}

void MainWindow::showWorkMode() {
  QString msg = QString("Current work mode:")
      + (work_mode == INSTANT?"Instant":"Final");
  switch (result_mode) {
    case DEC:
      msg += " Dec";
      break;
    case HEX:
      msg += " Hex";
      break;
    case OCT:
      msg += " Oct";
      break;
    case BIN:
      msg += " Bin";
      break;
  }
  ui->statusBar->showMessage(msg);
}

MainWindow::~MainWindow() {
  delete exp_buffer_;
  delete ui;
}

void MainWindow::on_actionCopy_the_result_triggered()
{
  QString tmp = QString::fromStdString(doubleToString(
                                         ui->result_display->value()));
  QApplication::clipboard()->setText(tmp);
  ui->statusBar->showMessage(tmp + " copied");
}

void MainWindow::on_input_box_textEdited(const QString &arg1)
{
  if(arg1.size() !=exp_buffer_->size()+1 && arg1.size() !=exp_buffer_->size()-1) {
    ui->input_box->setText(*exp_buffer_);
    return;
  } //deny none insert or backspace changes
  for(int i=0;i!=(arg1.size()>exp_buffer_->size()?exp_buffer_->size():
    arg1.size());++i)
      if(arg1[i] != (*exp_buffer_)[i]) {
        ui->input_box->setText(*exp_buffer_);
        return;
      }
  // allow insert only to back

  if (arg1.size() < exp_buffer_->size())
    emit(backspace()); // backspace
  else {
    if(arg1.size()>0) {
      QChar tmp = *(arg1.end()-1);
      QString tmp2(tmp);
      if(isOperator(tmp)) {
         addOperator(tmp2);
         ui->input_box->setText(*exp_buffer_);
      } else
         switch(tmp.toAscii()) {
           case '(':
             emit(left_button_clicked());
             ui->input_box->setText(*exp_buffer_);
             break;
           case ')':
             emit(right_button_clicked());
             ui->input_box->setText(*exp_buffer_);
             break;
           case '=':
             emit(equal_button_clicked());
             ui->input_box->setText(*exp_buffer_);
             break;
           case '0': case '1': case '2': case '3': case '4': case '5': case '6':
             case '7': case '8': case '9': case '.':
             addNum(tmp2);
             ui->input_box->setText(*exp_buffer_);
             break;
           default:
             ui->input_box->setText(*exp_buffer_);
             break;
         }
      }
  }
}

void MainWindow::addOperator(QString &operation_string) {
  if (exp_status_.operator_used) {
    emit(backspace());
    addOperator(operation_string);
    return; // avoid duplicate operator
  }

  if (!exp_buffer_->isEmpty())
      if (operation_string=="!" && *(exp_buffer_->end()-1)=='!')
        return;

  if (exp_status_.dot_used && !exp_status_.post_dot_number)//no number after dot
    *exp_buffer_ += "0"; // add 0 after dot

  if (work_mode == INSTANT) {
    if (exp_buffer_->isEmpty() && operation_string == "-") {
      *exp_buffer_ += operation_string;
      ui->input_box->setText(*exp_buffer_);
    } else   {
      *exp_buffer_ += "=";
      try {
        std::string tmp = exp_buffer_->toStdString();
        cCore calculator(tmp);
        result_buffer_ = calculator.calculate();
        ui->result_display->display(result_buffer_);
        *exp_buffer_ = QString::fromStdString(doubleToString(result_buffer_));
        *exp_buffer_ += operation_string;
        ui->input_box->setText(*exp_buffer_);
      } catch(const std::exception &e) {
        emit(clear_input());
        ui->statusBar->showMessage(e.what());
      }
    }
  } else {
    if (operation_string=="^" && *(exp_buffer_->end()-1)=='!')
      return;
    if(operation_string=="!" && *(exp_buffer_->end()-1)!=')') {
      int i;
      for(i=exp_buffer_->size()-1;i>=0;--i)
        if(isOperator((*exp_buffer_)[i]))
           break;
      exp_buffer_->insert(i+1,'(');
      exp_buffer_->append(')');
    }
    *exp_buffer_ += operation_string;
    ui->input_box->setText(*exp_buffer_);
  }

  exp_status_.dot_used = false;
  exp_status_.post_dot_number = false;
  if (operation_string != "!")
    exp_status_.operator_used = true;
  // change expression status accordingly
  showWorkMode();
}

void MainWindow::addNum(QString &num_string) {
  if (!exp_status_.dot_used || num_string!=".") {
    if(!exp_buffer_->isEmpty()) {
      // not first number
      if(*(exp_buffer_->end()-1)=='!')
        return;
      if(*(exp_buffer_->end()-1)==')')
        return;
      *exp_buffer_ += num_string;
      ui->input_box->setText(*exp_buffer_);
    } else {
      // first number
      *exp_buffer_ += num_string;
      ui->input_box->setText(*exp_buffer_);
    }
  }
  if(num_string==".")
    exp_status_.dot_used = true;
  if(exp_status_.dot_used && num_string!=".") // it's a post dot number
    exp_status_.post_dot_number = true;

  exp_status_.operator_used = false;
  showWorkMode();
}
void MainWindow::on_actionAbout_triggered()
{
  emit(wtf());
}

void MainWindow::on_actionDec_triggered()
{
  result_mode = DEC;
  ui->result_display->setMode(QLCDNumber::Dec);
  showWorkMode();
}

void MainWindow::on_actionOct_triggered()
{
  result_mode = OCT;
  ui->result_display->setMode(QLCDNumber::Oct);
  showWorkMode();
}

void MainWindow::on_actionHex_triggered()
{
  result_mode = HEX;
  ui->result_display->setMode(QLCDNumber::Hex);
  showWorkMode();
}

void MainWindow::on_actionBin_triggered()
{
  result_mode = BIN;
  ui->result_display->setMode(QLCDNumber::Bin);
  showWorkMode();
}

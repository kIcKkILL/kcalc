#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
  class MainWindow;
}

class QPushButton;
class QString;

class MainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  
private slots:
  void changeWorkMode();
  void num_button_clicked();
  void operator_button_clicked();
  void equal_button_clicked();
  void left_button_clicked();
  void right_button_clicked();
  void clear_input();
  void backspace();
  void wtf();

  void on_actionCopy_the_result_triggered();

  void on_input_box_textEdited(const QString &arg1);

  void on_actionAbout_triggered();

  void on_actionDec_triggered();

  void on_actionOct_triggered();

  void on_actionHex_triggered();

  void on_actionBin_triggered();

private:
  enum WorkMode { INSTANT, FINAL };
  enum ResultMode { DEC, OCT, HEX, BIN };
  WorkMode work_mode;
  ResultMode result_mode;
  // two work mode

  Ui::MainWindow *ui;
  QPushButton *num_button[10],*pi_button;
  QPushButton *point_button,*equal_button,*plus_button,*minus_button,
    *multiply_button,*division_button,*mod_button,*factorial_button,
    *power_button,*clear_button,*backspace_button,*left_button,
    *right_button;
  QPushButton *foo_button;
  void showWorkMode();
  // all buttons

  void addNum(QString &num_string);
  void addOperator(QString &operation_string);

  QString *exp_buffer_;
  QChar input_buffer;
  // buff last inputed char for availability check
  // temporary depraved, might be deleted if no use would be found
  double result_buffer_;

  struct ExpStatus {
    bool dot_used;
    bool post_dot_number;
    bool operator_used;
    ExpStatus():dot_used(false),post_dot_number(false),operator_used(false)
      {}
    void clear() { ExpStatus b; *this = b; }
  };
  ExpStatus exp_status_;
};

#endif // MAINWINDOW_H

class MainBox : public QWidget
{
   Q_OBJECT

   public:
       explicit MainBox(QWidget *parent);
       ~MainBox();

   private:
       void init();
       void function()
       {
           int x = 5;
       }
};

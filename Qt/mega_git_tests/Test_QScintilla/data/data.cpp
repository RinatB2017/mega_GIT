class MainBox : public QWidget
{
   Q_OBJECT

   public:
       explicit MainBox(QWidget *parent);
       ~MainBox();

   private:
       void init(void);
       void function(void)
       {
           int x = 5;
       }
};

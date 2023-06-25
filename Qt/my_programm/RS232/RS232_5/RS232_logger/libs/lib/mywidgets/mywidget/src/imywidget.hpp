
class iMyWidget
{
public:
    virtual void updateText(void) = 0;
    virtual bool programm_is_exit(void) = 0;
    virtual void load_setting(void) = 0;
    virtual void save_setting(void) = 0;
};

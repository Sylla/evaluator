#include "statistic.h"

statistic_::statistic_(): Level(CRITICAL_MODE)
{
    memset((void *) cnt, 0, sizeof(cnt));
}
//-----------------------------------------------------------------------------
void
statistic_::incrementCounter(unsigned short int type)
{
    mutex.lock();
    ++(cnt[type]);
    mutex.unlock();
}
//-----------------------------------------------------------------------------
void
statistic_::decrementCounter(unsigned short int type)
{
    mutex.lock();
    --(cnt[type]);
    mutex.unlock();
}
//-----------------------------------------------------------------------------
double
statistic_::getCounter(unsigned short int type)
{
    return cnt[type];
}
//-----------------------------------------------------------------------------
double
statistic_::setCounter(unsigned short int type, double value)
{
    return cnt[type]=value;
}
//-----------------------------------------------------------------------------
void
statistic_::messageOut(const short unsigned int Level_, const char *func, const char *message)
{
    if (Level >= Level_)
    {
        mutex.lock();
        std::cout << func << " :: "<< message<<std::endl;
        mutex.unlock();
    }
}
//-----------------------------------------------------------------------------
//friend ostream &operator<<(ostream&, const char *str);
//{
//    output << func << ": "<< message<<std::endl;
//}

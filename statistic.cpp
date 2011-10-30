#include "statistic.h"

statistic_::statistic_(): Level(CRITICAL_MODE)
{
    memset((void *) cnt, 0, sizeof(cnt));
}
//-----------------------------------------------------------------------------

void
statistic_::incrementCounter(unsigned short type)
{
    mutex.lock();
    ++(cnt[type]);
    mutex.unlock();
}
//-----------------------------------------------------------------------------
unsigned long int
statistic_::getCounter(unsigned short type)
{
    return cnt[type];
}
//-----------------------------------------------------------------------------
unsigned long int
statistic_::setCounter(unsigned short type, unsigned long value)
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

# include <sstream>
# include <QDateTime>
#include "TimeInfo.h"

# ifndef _WIN32
# include <sys/time.h>
# endif
/**
 * A constructor.
 * A more elaborate description of the constructor.
 */
TimeInfo::TimeInfo()
{
    setCurrent();
}

/**
 * A destructor.
 * A more elaborate description of the destructor.
 */
TimeInfo::~TimeInfo()
{
}


//**************************************************************************
// separator for other implementation aspects

void TimeInfo::setCurrent()
{
#if defined(_WIN32)
    _ftime(&timebuffer);
#else
    struct timeval t;
    gettimeofday(&t, nullptr);
    timebuffer.time = t.tv_sec;
    timebuffer.millitm = t.tv_usec / 1000;
#endif
}

void TimeInfo::setTime_t (int64_t seconds)
{
    timebuffer.time = seconds;
}

std::string TimeInfo::currentDateTimeString()
{
    return QDateTime::currentDateTime().toTimeSpec(Qt::OffsetFromUTC)
        .toString(Qt::ISODate).toStdString();
}

std::string TimeInfo::diffTime(const TimeInfo &timeStart,const TimeInfo &timeEnd )
{
   std::stringstream str;
   str << diffTimeF(timeStart,timeEnd);
   return str.str();
}

float TimeInfo::diffTimeF(const TimeInfo &timeStart,const TimeInfo &timeEnd )
{
    int64_t ds = int64_t(timeEnd.getSeconds() - timeStart.getSeconds());
    int dms = int(timeEnd.getMiliseconds()) - int(timeStart.getMiliseconds());

    return float(ds) + float(dms) * 0.001f;
}

TimeInfo TimeInfo::null()
{
    TimeInfo ti;
    ti.timebuffer = {};
    return ti;
}

bool TimeInfo::isNull() const
{
    return (*this) == TimeInfo::null();
}

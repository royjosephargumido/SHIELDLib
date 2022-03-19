#pragma once
#define SEARCH_H

class TranscriptSearch
{
    public:
        int getCS3P_StartDate(int key);
        int getCS3P_EndDate(int key);

    private:
        bool _isCSNExisting(int key);
        int _inRange(int key);

}
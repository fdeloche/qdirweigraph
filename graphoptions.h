#ifndef GRAPHOPTIONS_H
#define GRAPHOPTIONS_H


#define BLACKGRAPH 2
#define GREENRED 1
#define BLUEPURPLE 0

class graphOptions
{
public:
    graphOptions();

public:
    int style = BLUEPURPLE;
    int curve = 40;
    bool displayLabels = true;
    bool displayTitle = false;
};

#endif // GRAPHOPTIONS_H

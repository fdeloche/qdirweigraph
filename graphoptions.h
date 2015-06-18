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
    bool displayLabels = true;
    bool displayTitle = false;
};

#endif // GRAPHOPTIONS_H

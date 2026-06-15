#ifndef __DEMO_WIDGET_HPP__
#define __DEMO_WIDGET_HPP__

typedef struct nk_context;

class DemoWidget {


public:
    void Iterate(struct nk_context * const ctx);

    inline int GetSliderValue() const { return _sliderValue; }

private:
    int _sliderValue = 15;

};

#endif
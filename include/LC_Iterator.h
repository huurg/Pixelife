#ifndef LC_ITERATOR_HEADER
#define LC_ITERATOR_HEADER

struct LC_Iterator {
    // Properties
    bool env_periodic_boundary = 0;
    int env_w = 0;
    int env_h = 0;

    // Methods
    LC_Iterator();
    ~LC_Iterator();
    virtual bool iToXY(int i, int* ox, int* oy) const;
    virtual int getN() const;
};

#endif // LC_ITERATOR_HEADER

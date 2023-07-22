//
//  fhn_rk4.hpp
//  rk4
//
//  Created by Jeremy Bai on 17/07/2023.
//

#ifndef fhn_rk4_hpp
#define fhn_rk4_hpp

#include <iostream>
#include <vector>
#include <cmath>

class FhnSolver
{
public:
    
    FhnSolver(double samplerate) : dt(1.0/samplerate) {}
    ~FhnSolver() {}
    
    struct State
    {
        double v;
        double w;
        
        State() : v(0.0), w(0.0) {}
    };
    
    struct Delta
    {
        double dv;
        double dw;
        
        explicit Delta() : dv(0.0), dw(0.0) {}
        explicit Delta(double p, double q) : dv(p), dw(q) {}
        
        Delta operator/(const double x) const
        {
            return Delta(dv/x, dw/x);
        }
        
        Delta operator+(const Delta& d) const
        {
            return Delta(dv + d.dv, dw + d.dw);
        }
    };
    
    void setCurrentState(double newv, double neww)
    {
        currentState.v = newv;
        currentState.w = neww;
    }
    
    State updateCurrentState(Delta delta)
    {
        State newState;
        newState.v = currentState.v + delta.dv;
        newState.w = currentState.w + delta.dw;
        return newState;
    }
    
    void setParameter(double newa, double newb, double newc)
    {
        a = newa;
        b = newb;
        c = newc;
    }
    
    void setTemporalScale(double newk)
    {
        k = newk;
    }
    
    void setDt(double newdt)
    {
        dt = newdt;
    }
    
    Delta dy(State state)
    {
        Delta newDelta;
        newDelta.dv = (state.v - 25.0/12.0 * pow(state.v, 3) - 0.4 * state.w + 0.4 * currentInput) * dt * k;
        newDelta.dw = (2.5 * state.v + a - b * state.w) * c * dt * k;
        return newDelta;
    }
    
    void processSystem(double input)
    {
        currentInput = input;
        k1 = dy(currentState);
        k2 = dy(updateCurrentState(k1/2));
        k3 = dy(updateCurrentState(k2/2));
        k4 = dy(updateCurrentState(k3));
        currentState = updateCurrentState((k1 + k2/2 + k3/2 + k4)/6);
    }
    
    double getCurrentState()
    {
        return currentState.v;
    }

private:
    State currentState;
    Delta k1, k2, k3, k4;
    double currentInput = 0;
    double dt;
    double a = 0.7, b = 0.8, c = 0.1, k = 1;
    
};

#endif /* fhn_rk4_hpp */

#pragma once
#include <unordered_map>
#include <vector>
#include <functional>

template <typename TState>
class StateMachine
{
public:
    template <typename T, typename U>
    struct states
    {
        T start;
        U final;

        bool operator==(const states &rhs) const {
            return (this->start == rhs.start && this->final == rhs.final);
        }
    };

    template <typename T, typename U>
    struct statesHash {
        std::size_t operator()(const states<T, U>& p) const {
            return std::hash<T>()(p.start);
        }
    };

    StateMachine() {
        _curState = nullptr;
    }
    void addState(const TState& state) {
        if (!_curState) {
            _curState = new TState(state);
        }
        _states[state];
    };
    void addAction(const TState& state, const std::function<void()>& lambda) {
         _states[state].push_back(&lambda);
    };
    void addTransition(const TState& startState,
                        const TState& finalState,
                        const std::function<void()>& lambda) {
        _statesTransitions[{startState, finalState}].push_back(&lambda);
    }
    void transitionTo(const TState& state) {

       {
            auto it = _states.find(state);

            if (it == _states.end()) {
                throw std::invalid_argument("Exception: There is no such state");
            }
        }

        if (*_curState == state) {
            this->update();
            return ;
        }
    
        auto it = _statesTransitions.find({*_curState, state});

        delete _curState;
        _curState = new TState(state);
        if (it == _statesTransitions.end()) {
            throw std::invalid_argument("Exception: Handle state not found");
        }

        for (auto func : it->second) {
            (*func)();
        }

    };
    void update() {
        for (auto func : _states[*_curState]) {
            (*func)();
        }
    };
private:
    std::unordered_map<TState, std::vector<const std::function<void()> *>> _states;
    std::unordered_map<states<TState, TState>, std::vector<const std::function<void()> *>,
        statesHash<TState, TState>> _statesTransitions;
    TState *_curState;
};
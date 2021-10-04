#include <iostream>
#include <vector>
#include <cmath>

#define __infinity -1 // ---inf
#define infinity__ 1 // +++inf
#define closed_interval 0
#define inf_val 42 //

#define delta 1 // величина сдвига при локализации корня

//#define debug

long double a, b, c;
long double eps;

long double f(long double x) { // f(x)
    return (x * x * x) + (a * x * x) + (b * x) + c;
}

long double df_dx(long double x) { // f'(x)
    return 3 * x * x + 2 * a * x + b;
}

long double d2f_dx2(long double x) { // f''(x)
    return 6 * x + 2 * a;
}

static long double D_dfdx() { // D(f')
    return 4 * a * a - 12 * b;
}

struct interval {
    long double l;
    long double r;
    bool left_Inf;
    bool right_Inf;

    interval(long double l, long double r, int limitIndicator) {
        this->l = l;
        this->r = r;

        switch (limitIndicator) {
            case (__infinity) : {
                left_Inf = true;
                right_Inf = false;
                break;
            }
            case (infinity__) : {
                left_Inf = false;
                right_Inf = true;
                break;
            }
            case (closed_interval) : {
                left_Inf = false;
                right_Inf = false;
            }
        }
    }

    long double length() const {
        return r - l;
    }

    bool isUnlimited() const {
        return right_Inf || left_Inf;
    }

    bool isPoint() const {
        return l == r;
    }
};

void limitInterval(interval &intervalLocal) { // работает только для открытых с одной стороны интервалов
    if (intervalLocal.left_Inf) { // (-inf, t)
        while (intervalLocal.isUnlimited()) {
            long double tmp = intervalLocal.r - delta;
            if (f(tmp) <= -eps) {
                intervalLocal.l = tmp;
                intervalLocal.left_Inf = false;
            } else if (f(tmp) >= eps) {
                intervalLocal.r = tmp;
            } else {
                intervalLocal.l = tmp;
                intervalLocal.r = tmp;
                intervalLocal.left_Inf = false;
            }
        }
    } else { // (t, inf+)
        while (intervalLocal.isUnlimited()) {
            long double tmp = intervalLocal.l + delta;
            if (f(tmp) <= -eps) {
                intervalLocal.l = tmp;
            } else if (f(tmp) >= eps) {
                intervalLocal.r = tmp;
                intervalLocal.right_Inf = false;
            } else {
                intervalLocal.l = tmp;
                intervalLocal.r = tmp;
                intervalLocal.right_Inf = false;
            }
        }
    }
}

long double findRoot(interval intervalLocal) {
    if (intervalLocal.isPoint()) {
        return intervalLocal.r;
    }

    if (intervalLocal.isUnlimited()) {
        limitInterval(intervalLocal);
    }

    long double tmp = (intervalLocal.l + intervalLocal.r) / 2;

    if (f(intervalLocal.l) <= -eps && f(intervalLocal.r) >= eps) {
        while (std::abs(f(tmp)) >= eps) { /// asdaaasds
            if (f(tmp) <= -eps) {
                intervalLocal.l = tmp;
            } else {
                intervalLocal.r = tmp;
            }
            tmp = (intervalLocal.l + intervalLocal.r) / 2;
        }
    } else if (f(intervalLocal.l) >= eps && f(intervalLocal.r) <= -eps) {
        while (std::abs(f(tmp)) >= eps) {
            if (f(tmp) <= -eps) {
                intervalLocal.r = tmp;
            } else {
                intervalLocal.l = tmp;
            }
            tmp = (intervalLocal.l + intervalLocal.r) / 2;
        }
    } else {
        perror("\nCHTO TO NE TAK Y VAS S INTERVALAMY\n");
        printf("\n l = %Lg, r = %Lg\n", intervalLocal.l, intervalLocal.r);
        exit(212);
    }

    return tmp;
}

std::vector<interval> localizeRoots() {
#ifdef debug
    std::cout << "D_dfdx: " << D_dfdx() << std::endl;
#endif

    if (D_dfdx() <= -eps) {
        if (f(0) >= eps) {
            return {{inf_val, 0, __infinity}};
        } else if (f(0) <= -eps) {
            return {{0, inf_val, infinity__}};
        } else {
            return {{0, 0, closed_interval}};
        }
    } else if (D_dfdx() < eps) {
        long double omega = (-2 * a) / 6; // единственный экстремум производной
        if (f(omega) >= eps) {
            return {{inf_val, omega, __infinity}};
        } else if (f(omega) <= -eps) {
            return {{omega, inf_val, infinity__}};
        } else {
            return {{omega, omega, closed_interval}};
        }
    } else { // 2 корня у функции производной
        long double alpha = ((-2 * a) + std::sqrt(D_dfdx())) / 6; // т.к. такой вид куб уравн
        long double beta = ((-2 * a) - std::sqrt(D_dfdx())) / 6;
        if (alpha > beta) std::swap(alpha, beta);

        long double f_alpha = f(alpha);
        long double f_beta = f(beta);

#ifdef debug
        std::cout << "alpha, beta: " << alpha << ", " << beta << std::endl;
        std::cout << "f_alpha, f_beta: " << f_alpha << ", " << f_beta << std::endl;
#endif

        if (f_alpha >= eps && f_beta <= -eps) { // случай 3х корней
            return {{inf_val, alpha, __infinity},
                    {alpha,   beta,  closed_interval},
                    {beta, inf_val,  infinity__}};
        } else if (std::abs(f_alpha) < eps && f_beta <= -eps) { // 2 корня сл 1
            return {{alpha, alpha,  closed_interval},
                    {beta, inf_val, infinity__}};
        } else if (f_alpha >= eps && std::abs(f_beta) < eps) { // 2 корня сл 2
            return {{inf_val, alpha, __infinity},
                    {beta,    beta,  closed_interval}};
        } else if (f_alpha <= -eps && f_beta <= -eps) { // случай 1-го корня
            return {{beta, inf_val, infinity__}};
        } else if (f_alpha >= eps && f_beta >= eps) {
            return {{inf_val, alpha, __infinity}};
        } else {
            perror("\nK ETOMY MENYA ZHIZN NE GOTOVILA\n");
            exit(228);
        }
    }
}

int calcMultiplicityOfRoot(long double root) {
    int multiplicity = 1;

    if (std::abs(df_dx(root)) < eps) {
        ++multiplicity;
    }
    if (std::abs(d2f_dx2(root)) < eps) {
        ++multiplicity;
    }

    return multiplicity;
}

int main() {
    scanf("%Lg%Lg%Lg", &a, &b, &c);
    scanf("%Lg", &eps);

    std::vector<interval> intervalsLocalization = localizeRoots();

    printf("\nRoots:\n");
    for (auto &intervalLocal: intervalsLocalization) {
        long double root = findRoot(intervalLocal);
        printf("Root: %Lg, Multiplicity: %d\n", root, calcMultiplicityOfRoot(root));
    }
}

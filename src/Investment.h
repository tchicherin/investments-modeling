//
// Created by Timofey Chicherin on 15.10.2025.
//

#ifndef IINVESTMENT_H
#define IINVESTMENT_H

class IInvestment {
 public:
  virtual ~IInvestment() = default;

  virtual void UpdateToNextMonth() = 0;

  virtual void SetInterest(double value) { interest_ = value; }
  virtual double GetInterest() const { return interest_; }

  virtual void SetVolatility(double value) { volatility_ = value; }
  virtual double GetVolatility() const { return volatility_; }


 protected:
  double interest_ = 0, volatility_ = 0;
};

#endif // IINVESTMENT_H

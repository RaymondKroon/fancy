/*
 *   Copyright (c) 2015 Raymond Kroon. All rights reserved.
 *   The use and distribution terms for this software are covered by the
 *   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
 *   which can be found in the file LICENSE.txt at the root of this distribution.
 *   By using this software in any fashion, you are agreeing to be bound by
 *   the terms of this license.
 *   You must not remove this notice, or any other, from this software.
 */


#ifndef PUNCH_LANG_VISITORS_HPP
#define PUNCH_LANG_VISITORS_HPP

#include <type_traits>
#include <lang/expression.hpp>
#include <lang/util.hpp>

namespace ple = punch::lang::expressions;

namespace punch{
  namespace lang {

    namespace expressions {
      class Keyword;
      class Integer;
      class Float;
      class Ratio;
      class Symbol;
      class Symbolic;
      class Map;
      class Set;
      class String;
      class Vector;
    }

    using namespace expressions;


    class ExpressionVisitor {
    public:
      virtual ~ExpressionVisitor() { };

      virtual void visit(Expression const * const) = 0;

      virtual void before(Keyword const &val) {};
      virtual void after(Keyword const &val) {};

      virtual void before(Integer const &val) {};
      virtual void after(Integer const &val) {};

      virtual void before(Float const &val) {};
      virtual void after(Float const &val) {};

      virtual void before(Ratio const &val) {};
      virtual void after(Ratio const &val) {};

      virtual void before(Symbol const &val) {};
      virtual void after(Symbol const &val) {};

      virtual void before(Symbolic const &val) {};
      virtual void after(Symbolic const &val) {};

      virtual void before(Map const &val) {};
      virtual void after(Map const &val) {};

      virtual void before(Set const &val) {};
      virtual void after(Set const &val) {};

      virtual void before(String const &val) {};
      virtual void after(String const &val) {};

      virtual void before(Vector const &val) {};
      virtual void after(Vector const &val) {};

    private:
      virtual ExpressionVisitor* derived()  = 0;
    };


    class MutatingExpressionVisitor {
    public:
      virtual ~MutatingExpressionVisitor() { };

      virtual void visit(Expression const *) = 0;

    private:
      virtual MutatingExpressionVisitor* derived() = 0;
    };

    template <typename Derived>
    class RecursiveVisitor : public ExpressionVisitor {
    public:

      RecursiveVisitor(int starting_level) : level(starting_level) {}

      void visit(Expression const * expression) {
        if (expression) {
          handle_expression(expression);
        }
      }

    protected:
      int level = 0;

    private:

      ExpressionVisitor* derived() {
        return static_cast<Derived*>(this);
      }

      template <typename e_type>
      e_type const * _cast(Expression const * expression) {
        return as_type<Expression, e_type>(expression);
      }

      void handle_expression(Expression const * expression) {
        if (expression->type() == ExpressionType::Keyword) {
          auto expr = _cast<Keyword>(expression);

          derived()->before(*expr);
          derived()->after(*expr);
        }
        else if (expression->type() == ExpressionType::Integer) {
          auto expr = _cast<Integer>(expression);

          derived()->before(*expr);
          derived()->after(*expr);
        }
        else if (expression->type() == ExpressionType::Float) {
          auto expr = _cast<Float>(expression);

          derived()->before(*expr);
          derived()->after(*expr);
        }
        else if (expression->type() == ExpressionType::Ratio) {
          auto expr = _cast<Ratio>(expression);

          derived()->before(*expr);
          derived()->after(*expr);
        }
        else if (expression->type() == ExpressionType::Symbol) {
          auto expr = _cast<Symbol>(expression);

          derived()->before(*expr);
          derived()->after(*expr);
        }
        else if (expression->type() == ExpressionType::Symbolic) {
          level++;

          auto expr = _cast<Symbolic>(expression);

          derived()->before(*expr);

          auto inner = expr->get_inner();
          for (auto it = inner->begin(); it != inner->end(); ++it) {
            (*it)->accept(*this);
          }

          derived()->after(*expr);
          level--;
        }
        else if (expression->type() == ExpressionType::Map) {
          level++;

          auto expr = _cast<Map>(expression);

          derived()->before(*expr);

          auto inner = expr->get_inner();
          auto it = inner->begin();

          while (it != inner->end()) {

            (*it)->accept(*this);
            ++it;
            (*it)->accept(*this);

          }

          derived()->after(*expr);
          level--;
        }
        else if (expression->type() == ExpressionType::Set) {
          level++;

          auto expr = _cast<Set>(expression);

          derived()->before(*expr);

          auto inner = expr->get_inner();
          for (auto it = inner->begin(); it != inner->end(); ++it) {
            (*it)->accept(*this);
          }

          derived()->after(*expr);
          level--;
        }
        else if (expression->type() == ExpressionType::String) {
          auto expr = _cast<String>(expression);

          derived()->before(*expr);
          derived()->after(*expr);
        }
        else if (expression->type() == ExpressionType::Vector) {
          level++;

          auto expr = _cast<Vector>(expression);

          derived()->before(*expr);

          auto inner = expr->get_inner();
          for (auto it = inner->begin(); it != inner->end(); ++it) {
            (*it)->accept(*this);
          }

          derived()->after(*expr);
          level--;
        }
      }
    };

    #define Derive_RecursiveVisitor(Type) class Type: public RecursiveVisitor<Type>

    Derive_RecursiveVisitor(LoggingVisitor) {

    public:

      LoggingVisitor() : LoggingVisitor(std::cout, 0){}
      LoggingVisitor(std::ostream &os, int starting_level) : RecursiveVisitor(starting_level), os(os) {}

      void before(Keyword const &val) override;
      void before(Integer const &val) override;
      void before(Float const &val) override;
      void before(Ratio const &val) override;
      void before(Symbol const &val) override;
      void before(Symbolic const &val) override;
      void after(Symbolic const &val) override;
      void before(Map const &val) override;
      void after(Map const &val) override;
      void before(Set const &val) override;
      void after(Set const &val) override;
      void before(String const &val) override;
      void before(Vector const &val) override;
      void after(Vector const &val) override;

    private:

      void print(std::string s) {
        if (!no_space) {
          os << " ";
        }

        if (no_space) {
          no_space = false;
        }

        os << s;
        if (level == 0) {
          os << std::endl;
        }
      }

      bool no_space = false;
      std::ostream& os;

    };
  }
}

#endif //PUNCH_LANG_VISITORS_HPP

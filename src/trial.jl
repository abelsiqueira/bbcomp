function safeLogin()
  return
end

function safeSetTrack()
  return
end

function safeGetNumberOfProblems()
  return 10
end

function safeSetProblem(pid)
  return
end

function safeGetBudget(pid)
  return 100*safeGetDimension(pid)^2
end

function safeGetDimension(pid)
  return 3*(pid+1)
end

function safeGetEvaluations(pid)
  return 0
end

function safeEvaluate(pid, point)
  dim = safeGetDimension(pid)
  f = 0.0
  n = 0.0
  for i = 1:dim
    x = point[i]-0.5
    xhat = x > 0 ? log(abs(x)) : 0
    c1 = x > 0 ? 10 : 5.5
    c2 = x > 0 ? 7.9 : 3.1
    x2 = sign(x)*exp(xhat+0.049*(sin(c1*xhat)+sin(c2*xhat)))
    if x2 > 0
      x = x2^(1+0.2*(i-1)/(dim-1)*sqrt(x2))
    else
      x = x2
    end
    x *= 10^(0.5*(i-1)/(dim-1))
    f += x^2
    n += cos(2*pi*x)
  end
  return f + 10*(dim-n)
end

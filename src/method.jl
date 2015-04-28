function solveProblem(pID)
  pID = Cint(pID)
  safeSetProblem(pID)

  bud = safeGetBudget(pID)
  dim = safeGetDimension(pID)
  evals = safeGetEvaluations(pID)

  best = zeros(dim)
  best_f = Inf
  point = zeros(dim)

  center = 0.5*ones(dim)
  radius = 0.5

  # Random Search
  rand_search_bud = 0.0*bud
  while evals < rand_search_bud
    for i = 1:dim
      point[i] = max(0.0, min(1.0, center[i] + (2*rand()-1)*radius))
    end
    f = safeEvaluate(pID, point)
    if f < best_f
      copy!(best, point)
      best_f = f
      copy!(center, best)
    end
    evals += 1
    radius *= 0.95
  end

  # Local Search
  delta = 0.1
  copy!(point, best)
  f_curr = best_f
  while evals < bud
    ibest = 0
    best_f = f_curr
    for i = 1:dim
      point[i] += delta
      if point[i] < 0.0 || point[i] > 1.0
        f = Inf
      else
        f = safeEvaluate(pID, point)
      end
      if f < best_f
        best_f = f
        ibest = i
      end
      point[i] -= 2*delta
      if point[i] < 0.0 || point[i] > 1.0
        f = Inf
      else
        f = safeEvaluate(pID, point)
      end
      if f < best_f
        best_f = f
        ibest = -i
      end
      point[i] += delta
    end
    if ibest != 0 # lower value point found
      i = abs(ibest)
      s = ibest > 0 ? 1 : -1
      point[i] += s*delta
      f_curr = best_f
    else # every neighbour is worse
      delta *= 0.5
    end
    evals += 2
  end

  println("best_f = $best_f")
end

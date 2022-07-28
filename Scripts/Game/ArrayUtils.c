class RefArrayUtils<ref Class T> {
	static void Permute(out array<ref T> a)
	{
		foreach (int i, ref T x: a)
			a.SwapItems(i, a.GetRandomIndex());
	}

	static array<ref T> Choose(int n, array<ref T> pool, bool random = true)
	{
		array<ref T> result = new array<ref T>();
		result.Resize(n);
		Choose(n, pool, result);
		return result;
	}

	static void Choose(int n, array<ref T> pool, out array<ref T> result, bool random = true)
	{
		int count = pool.Count();
		for (int iResult = 0, iPool = 0; iResult < n; iResult++)
		{
			++iPool;
			if (iPool >= count) iPool = 0;

			ref T chosen;
			if (random) chosen = pool.GetRandomElement();
			else        chosen = pool[iPool];			

			result.Set(iResult, chosen);
		}
	}
}

class ArrayUtils<Class T> {
	static void Permute(out array<T> a)
	{
		foreach (int i, T x: a)
			a.SwapItems(i, a.GetRandomIndex());
	}

	static array<T> Choose(int n, array<T> pool, bool random = true)
	{
		array<T> result = new array<T>();
		result.Resize(n);
		Choose(n, pool, result);
		return result;
	}

	static void Choose(int n, array<T> pool, out array<T> result, bool random = true)
	{
		int count = pool.Count();
		for (int iResult = 0, iPool = 0; iResult < n; iResult++)
		{
			++iPool;
			if (iPool >= count) iPool = 0;

			T chosen;
			if (random) chosen = pool.GetRandomElement();
			else        chosen = pool[iPool];			

			result.Set(iResult, chosen);
		}
	}
}

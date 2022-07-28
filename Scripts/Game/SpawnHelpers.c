class SpawnHelpers {
	static ref RandomGenerator RNG = new RandomGenerator();
	
	static void SpawnRandomInRadius(Resource resource, vector spawnOrigin, float radius)
	{
		vector spawnPos = RNG.GenerateRandomPointInRadius(0, radius, spawnOrigin);
		
		if (!resource)
		{
			Print("No resource given.", LogLevel.WARNING);
			return;
		}
		
		EntitySpawnParams params = EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		params.Transform[3] = spawnPos;
		IEntity entity = GetGame().SpawnEntityPrefab(resource, null, params);
		
		if (!entity)
		{
			Print("Error: Could not create entity", LogLevel.WARNING);
			return;
		}

		GenericHelpers.SnapToTerrain(entity);
	}
	
	static void SpawnPoolInRadius(array<ref Resource> entities, int spawnCount, vector spawnOrigin, float radius, bool randomChoose = true)
	{
		int entityCount = entities.Count();
		for (int i = 0, g = 0; i < spawnCount; i++)
		{
			++g;
			if (g >= entityCount) g = 0;
	
			Resource entity;
			if (randomChoose) entity = entities.GetRandomElement();
			else              entity = entities[g];			

			SpawnRandomInRadius(entity, spawnOrigin, radius);
		}
	}
	
	static void SpawnPoolInRadius(array<ResourceName> entityNames, int spawnCount, vector spawnOrigin, float radius, bool randomChoose = true)
	{
		int entityCount = entityNames.Count();
		array<ref Resource> entities = new array<ref Resource>();

		foreach (string name: entityNames)
			entities.Insert(Resource.Load(name));
		
		SpawnPoolInRadius(entities, spawnCount, spawnOrigin, radius, randomChoose);
	}

	static void SpawnPoolInRadius(array<string> entityNames, int spawnCount, vector spawnOrigin, float radius, bool sequential = true)
	{
		int entityCount = entityNames.Count();
		array<ref Resource> entities = new array<ref Resource>();
		foreach (string name: entityNames) {
			entities.Insert(Resource.Load(name));
		}
		
		SpawnPoolInRadius(entities, spawnCount, spawnOrigin, radius, sequential);
	}
}

class SpawnHelpers {
	static ref RandomGenerator RNG = new RandomGenerator();

	static IEntity SpawnEntity(Resource resource, vector spawnPos)
	{		
		if (!resource)
		{
			Print("No resource given.", LogLevel.WARNING);
			return null;
		}
		
		
		EntitySpawnParams params = EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		params.Transform[3] = spawnPos;
		
		IEntity entity = GetGame().SpawnEntityPrefab(resource, null, params);
		
		if (!entity)
		{
			Print("Error: Could not create entity", LogLevel.WARNING);
			return null;
		}
		
		//entity.SetOrigin(spawnPos);
		entity.Update();
		return entity;
	}

	static IEntity SpawnRandomInRadius(Resource resource, vector spawnOrigin, float radius)
	{
		vector spawnPos = RNG.GenerateRandomPointInRadius(0, radius, spawnOrigin);

		IEntity entity = SpawnEntity(resource, spawnPos);		
		if (!entity) return null;

		GenericHelpers.SnapAndOrientToTerrain(entity);	
		return entity;
	}

	static array<ref Resource> ToResources(array<ResourceName> names)
	{
		int entityCount = names.Count();
		array<ref Resource> entities = new array<ref Resource>();
		entities.Resize(entityCount);

		foreach (int i, string name: names)
			entities.Set(i, Resource.Load(name));

		return entities;
	}

	static array<ref Resource> ToResources(array<string> names)
	{
		int entityCount = names.Count();
		array<ref Resource> entities = new array<ref Resource>();
		entities.Resize(entityCount);

		foreach (int i, string name: names)
			entities.Set(i, Resource.Load(name));

		return entities;
	}
	
	/* static array<IEntity> SpawnPoolInRadius(array<ref Resource> entities, int spawnCount, vector spawnOrigin, float radius, bool randomChoose = true) */
	static void SpawnPoolInRadius(array<ref Resource> entities, int spawnCount, vector spawnOrigin, float radius, bool randomChoose = true)
	{
		array<ref Resource> resources = RefArrayUtils<ref Resource>.Choose(spawnCount, entities, randomChoose);
		foreach (Resource res : resources)
		{
			IEntity e = SpawnRandomInRadius(res, spawnOrigin, radius);
			e.Update();
		}
	}
	
	static void SpawnPoolInRadius(array<ResourceName> entityNames, int spawnCount, vector spawnOrigin, float radius, bool randomChoose = true)
	{
		SpawnPoolInRadius(ToResources(entityNames), spawnCount, spawnOrigin, radius, randomChoose);
	}

	static void SpawnPoolInRadius(array<string> entityNames, int spawnCount, vector spawnOrigin, float radius, bool randomChoose = true)
	{
		SpawnPoolInRadius(ToResources(entityNames), spawnCount, spawnOrigin, radius, randomChoose);
	}
		
	static void SpawnLootboxPoolInRadius(array<ResourceName> entityNames, int spawnCount, vector spawnOrigin, float radius, ResourceName lootboxName, bool randomChoose = true)
	{
		Resource lootbox = Resource.Load(lootboxName);
		if (!lootbox) Print("Error: no lootbox value provided");

		auto resources = ArrayUtils<ref ResourceName>.Choose(spawnCount, entityNames, randomChoose);
		foreach (ResourceName name : resources)
		{
			Resource res = Resource.Load(name);
			IEntity table = SpawnRandomInRadius(lootbox, spawnOrigin, radius);
			table.Update();
			/* IEntity entity = SpawnEntity(res, table.GetOrigin()); */
			/* entity.Update(); */
		}
	}
}

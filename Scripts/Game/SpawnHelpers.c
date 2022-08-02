class SpawnHelpers {
	static ref RandomGenerator RNG = new RandomGenerator();

	static IEntity SpawnEntity(Resource resource, vector spawnPos, vector yawPitchRoll = "0 0 0", IEntity parent = null)
	{
		if (!resource)
		{
			Print("No resource given.", LogLevel.WARNING);
			return null;
		}

		ResourceName resourceName = resource.GetResource().GetResourceName();

		EntitySpawnParams params = new EntitySpawnParams();

		params.TransformMode = ETransformMode.WORLD;
		Math3D.AnglesToMatrix(yawPitchRoll, params.Transform);
		params.Transform[3] = spawnPos;

		if (parent)
			params.Parent = parent;

		IEntity entity = GetGame().SpawnEntityPrefab(resource, null, params);

		if (!entity)
		{
			Print("Error: Could not create entity for " + resourceName, LogLevel.ERROR);
			return null;
		}

		entity.Update();
		
		RplComponent rplComponent = RplComponent.Cast(entity.FindComponent(RplComponent));
		if (rplComponent)
			Print("Replication is" + rplComponent + " for " + resourceName, LogLevel.DEBUG);
		else
			Print("No replication for Resource '" + resourceName + "' Entity " + entity, LogLevel.WARNING);

		return entity;
	}

	static void AddRplComponent(Resource resource)
	{
		BaseContainer container = resource.GetResource().ToBaseContainer();
		ref BaseContainerList componentList = container.GetObjectArray("components");
		// TODO: Stub
	}

	static IEntity SpawnRandomInRadius(Resource resource, IEntity target, float radius, bool parent = false)
	{
		if (!parent) return SpawnRandomInRadius(resource, target.GetOrigin(), radius);

		vector origin = "0 0 0";
		vector spawnPos = RNG.GenerateRandomPointInRadius(0, radius, "0 0 0");
		vector yawPitchRoll = "1 0 0" * RNG.RandFloatXY(-180, 180);

		IEntity result = SpawnEntity(resource, spawnPos, yawPitchRoll, target);
		if (!result) return null;

		GenericHelpers.SnapAndOrientToTerrain(result);
		result.Update();
		return result;
	}

	static IEntity SpawnRandomInRadius(Resource resource, vector spawnOrigin, float radius)
	{
		vector spawnPos = RNG.GenerateRandomPointInRadius(0, radius, spawnOrigin);
		vector yawPitchRoll = "1 0 0" * RNG.RandFloatXY(-180, 180);

		IEntity entity = SpawnEntity(resource, spawnPos, yawPitchRoll);
		if (!entity) return null;

		GenericHelpers.SnapAndOrientToTerrain(entity);
		entity.Update();
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

	static array<IEntity> SpawnPoolInRadius(array<ref Resource> entities, int spawnCount, vector spawnOrigin, float radius, bool randomChoose = true)
	{
		array<ref Resource> resources = RefArrayUtils<ref Resource>.Choose(spawnCount, entities, randomChoose);
		array<IEntity> result = new array<IEntity>();
		result.Resize(spawnCount);

		foreach (int i, Resource res : resources)
		{
			IEntity ent = SpawnRandomInRadius(res, spawnOrigin, radius);
			result.Set(i, ent);
		}
		return result;
	}

	static array<IEntity> SpawnPoolInRadius(array<ResourceName> entityNames, int spawnCount, vector spawnOrigin, float radius, bool randomChoose = true)
	{
		array<ref Resource> res = ToResources(entityNames);
		array<IEntity> entities = SpawnPoolInRadius(res, spawnCount, spawnOrigin, radius, randomChoose);
		return entities;
	}

	static array<IEntity> SpawnPoolInRadius(array<string> entityNames, int spawnCount, vector spawnOrigin, float radius, bool randomChoose = true)
	{
		return SpawnPoolInRadius(ToResources(entityNames), spawnCount, spawnOrigin, radius, randomChoose);
	}

	static void SpawnLootboxPoolInRadius(array<ResourceName> entityNames, int spawnCount, vector spawnOrigin, float radius,
										 ResourceName lootboxName, bool randomChoose = true)
	{
		Resource lootbox = Resource.Load(lootboxName);
		if (!lootbox) Print("Error: no lootbox value provided");

		auto resources = ArrayUtils<ref ResourceName>.Choose(spawnCount, entityNames, randomChoose);
		foreach (ResourceName name : resources)
		{
			Resource res = Resource.Load(name);
			IEntity entity, table;

			entity = SpawnRandomInRadius(res, spawnOrigin, radius);
			entity.Update();

			table = SpawnEntity(lootbox, entity.GetOrigin(), entity.GetYawPitchRoll());
			table.Update();
		}
	}
}

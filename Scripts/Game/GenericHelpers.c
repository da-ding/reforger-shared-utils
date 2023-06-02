class Helpers
{
	static void SnapToTerrain(IEntity entity)
	{
		vector transform[4];
		entity.GetTransform(transform);

		SCR_TerrainHelper.SnapToTerrain(transform);
		entity.SetTransform(transform);
	}

	static void SnapAndOrientToTerrain(IEntity entity)
	{
		vector transform[4];
		entity.GetTransform(transform);

		SCR_TerrainHelper.SnapAndOrientToTerrain(transform);
		entity.SetTransform(transform);
	}

	static void FactionWin(Faction faction)
	{
		int factionIndex = GetGame().GetFactionManager().GetFactionIndex(faction);

		auto gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
		gameMode.EndGameMode(
				SCR_GameModeEndData.CreateSimple(SCR_GameModeEndData.ENDREASON_EDITOR_FACTION_VICTORY, -1, factionIndex)
		);
	}

	static IEntity GetVehicle(IEntity entity)
	{
		if (!entity) return null;
		auto compartmentAccessTarget = SCR_CompartmentAccessComponent.Cast(entity.FindComponent(SCR_CompartmentAccessComponent));
		if (compartmentAccessTarget)
		{
			return compartmentAccessTarget.GetVehicle();
		}
		return null;
	}
	
	static IEntity FindEntity(RplId id)
	{
		RplComponent rplC = RplComponent.Cast(Replication.FindItem(id));
		if (!rplC) return null;
		return rplC.GetEntity();
	}
}


class GenericHelpers : Helpers {}

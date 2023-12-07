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
				SCR_GameModeEndData.CreateSimple(EGameOverTypes.EDITOR_FACTION_VICTORY, -1, factionIndex)
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

	static RplId GetRplId(IEntity entity)
	{
		RplComponent rplC = RplComponent.Cast(entity.FindComponent(RplComponent));
		if (!rplC) return null;
		return rplC.Id();
	}

	static bool addAiToPlayerGroup(IEntity player, IEntity ai)
	{
		int playerId = GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(player);

		if (!playerId)
		{
			Print("DAD_JoinGroupAction: No player ID!", LogLevel.WARNING);
			return false;
		}

		SCR_AIGroup playerGroup = SCR_GroupsManagerComponent.GetInstance().GetPlayerGroup(playerId);

		if (!playerGroup)
		{
			Print("DAD_JoinGroupAction: No player group!", LogLevel.WARNING);
			return false;
		}

		return playerGroup.AddAIEntityToGroup(ai);
	}
}


class GenericHelpers : Helpers {}

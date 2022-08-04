[EntityEditorProps(category: "GameScripted/Triggers", description: "")]
class DAD_FactionControlTriggerEntityClass: DAD_BaseTriggerEntityClass
{
};

class DAD_FactionControlTriggerEntity: DAD_BaseTriggerEntity
{
	[Attribute("1", UIWidgets.ComboBox, "How should friendly ratio limit be evaluated.", category: "Faction Control Trigger", enums: { ParamEnum("More than", "0"), ParamEnum("Equals to", "1"), ParamEnum("Less than", "2") })]
	protected int m_iRatioMethod;

	[Attribute("0.5", UIWidgets.Slider, "Limit for how large portion of trigger entities are friendly.\n\nExamples:\n1 = Only friendlies are present\n0 = Only enemies are present\n0.5 = Equal number of friendlies and enemies\n\nEvaluated only when at least some friendlies or enemies are inside.\nWhen the trigger is empty, condition for ratio = 0 won't activate the trigger.", category: "Faction Control Trigger", params: "0 1 0.1")]
	protected float m_fFriendlyRatioLimit;

	protected ref map<Faction, int> m_mFactionCounts = new map<Faction, int>();

	protected ref array<Faction> m_aTriggeredFactions = new array<Faction>();

	override protected void EOnInit(IEntity owner)
	{
		FactionManager factionManager = GetGame().GetFactionManager();
		array<Faction> factions = new array<Faction>();
		factionManager.GetFactionsList(factions);
		
		foreach (Faction f : factions)
		{
			m_mFactionCounts.Insert(f, 0);
		} 
	}

	/*!
	Get number of entities inside trigger for each side
	\param[out] factionCounts An map of all factions and their corresponding counts
	*/
	void GetFactionCounts(out notnull map<Faction, int> factionCounts)
	{
		factionCounts.Copy(m_mFactionCounts);
	}

	/*!
	Get an array of factions that have currently triggered the control query.
	This may be multiple, and possibly even conficting sides (e.g. if a big faction is friendly to both, maybe. I don't know the inner workings of FactionManager)
	\param[out] triggeredFactions An array of all factions that triggered the most recent query
	*/
	void GetTriggeredFactions(out notnull array<Faction> triggeredFactions)
	{
		triggeredFactions.Copy(m_aTriggeredFactions);
	}

	protected void ClearFactions()
	{
		foreach (Faction fac, int count : m_mFactionCounts)
			m_mFactionCounts.Set(fac, 0);
	}

	override bool ScriptedEntityFilterForQuery(IEntity ent)
	{
		if (!m_mFactionCounts) return false; // not yet initialised
		if (!super.ScriptedEntityFilterForQuery(ent)) return false; // fails basic requirements

		//--- Querying itself - return results of cached faction counts
		if (ent == this)
		{
			m_aTriggeredFactions.Clear();

			foreach (Faction faction, int count : m_mFactionCounts)
			{
				if (count == 0) continue;

				int friendlyCount = 0;
				int enemyCount = 0;

				foreach (Faction opFaction, int opCount : m_mFactionCounts)
				{
					if (faction.IsFactionEnemy(opFaction)) enemyCount += opCount;
					if (faction.IsFactionFriendly(opFaction) || faction.GetFactionKey() == opFaction.GetFactionKey()) // TODO: is this redundant?
						friendlyCount += opCount;
					// Ignore neutral parties
				}

				float friendlyRatio = count / Math.Max(friendlyCount + enemyCount, 1);
				PrintFormat("%1: %2 / %3", faction.GetFactionKey(), friendlyRatio, m_fFriendlyRatioLimit);

				bool triggered = false;
				switch (m_iRatioMethod)
				{
					case 0:
						triggered = friendlyRatio > m_fFriendlyRatioLimit; break;
					case 1:
						triggered = float.AlmostEqual(friendlyRatio, m_fFriendlyRatioLimit); break;
					case 2:
						triggered = friendlyRatio < m_fFriendlyRatioLimit; break;
				}

				if (triggered)
					m_aTriggeredFactions.Insert(faction);
			}

			ClearFactions();
			return !m_aTriggeredFactions.IsEmpty();
		}

		//--- Evaluate engine-driven conditions, e.g., entity class
		if (!IsAlive(ent))
			return false;

		//--- Increase faction counters
		FactionAffiliationComponent factionAffiliation = FactionAffiliationComponent.Cast(ent.FindComponent(FactionAffiliationComponent));
		if (factionAffiliation)
		{
			Faction entFaction = factionAffiliation.GetAffiliatedFaction();
			if (entFaction)
				m_mFactionCounts.Set(entFaction, m_mFactionCounts.Get(entFaction) + 1);
		}

		return false;
	}
};

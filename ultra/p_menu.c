#include "g_local.h"

void PMenu_Open (edict_t *ent, pmenu_t *entries, int cur, int num)
{
	pmenuhnd_t *hnd;
	pmenu_t *p;
	int i;

	if (!ent->client)
		return;

	if (ent->client->menu) {
		gi.dprintf("warning, ent already has a menu\n");
		PMenu_Close(ent);
	}

//	hnd = malloc(sizeof(*hnd));
	hnd = gi.TagMalloc(sizeof(*hnd), TAG_LEVEL);

	hnd->entries = entries;
	hnd->num = num;

	if (cur < 0 || !entries[cur].SelectFunc) {
		for (i = 0, p = entries; i < num; i++, p++)
			if (p->SelectFunc)
				break;
	} else
		i = cur;

	if (i >= num)
		hnd->cur = -1;
	else
		hnd->cur = i;

	ent->client->showscores = true;
	ent->client->inmenu = true;
	ent->client->menu = hnd;

	PMenu_Update(ent);
	gi.unicast (ent, true);
}

void PMenu_Close (edict_t *ent)
{
	if (!ent->client->menu)
		return;

//	free(ent->client->menu);
	gi.TagFree(ent->client->menu);
	ent->client->menu = NULL;
	ent->client->showscores = false;
}

void PMenu_Update (edict_t *ent)
{
	char		string[1400];
	int			i;
	pmenu_t		*p;
	int			x;
	pmenuhnd_t	*hnd;
	char		*t;
	qboolean	alt = false;
	char		tmp[128];	// Knightmare added

	if (!ent->client->menu) {
		gi.dprintf("warning:  ent has no menu\n");
		return;
	}

	hnd = ent->client->menu;

//	strcpy(string, "xv 32 yv 8 picn inventory ");
	Com_strcpy(string, sizeof(string), "xv 32 yv 8 picn inventory ");

	for (i = 0, p = hnd->entries; i < hnd->num; i++, p++)
	{
		if (!p->text || !*(p->text))
			continue; // blank line
		t = p->text;
		if (*t == '*') {
			alt = true;
			t++;
		}
	//	sprintf(string + strlen(string), "yv %d ", 32 + i * 8);
		Com_sprintf (tmp, sizeof(tmp), "yv %d ", 32 + i * 8);
		Com_strcat (string, sizeof(string), tmp);
		if (p->align == PMENU_ALIGN_CENTER)
			x = 196/2 - (int)strlen(t)*4 + 64;
		else if (p->align == PMENU_ALIGN_RIGHT)
			x = 64 + (196 - (int)strlen(t)*8);
		else
			x = 64;

	//	sprintf(string + strlen(string), "xv %d ", x - ((hnd->cur == i) ? 8 : 0));
		Com_sprintf (tmp, sizeof(tmp), "xv %d ", x - ((hnd->cur == i) ? 8 : 0));
		Com_strcat (string, sizeof(string), tmp);

		if (hnd->cur == i) {
		//	sprintf(string + strlen(string), "string2 \"\x0d%s\" ", t);
			Com_sprintf (tmp, sizeof(tmp), "string2 \"\x0d%s\" ", t);
			Com_strcat (string, sizeof(string), tmp);
		}
		else if (alt) {
		//	sprintf(string + strlen(string), "string2 \"%s\" ", t);
			Com_sprintf (tmp, sizeof(tmp), "string2 \"%s\" ", t);
			Com_strcat (string, sizeof(string), tmp);
		}
		else {
		//	sprintf(string + strlen(string), "string \"%s\" ", t);
			Com_sprintf (tmp, sizeof(tmp), "string \"%s\" ", t);
			Com_strcat (string, sizeof(string), tmp);
		}
		alt = false;
	}

	gi.WriteByte (svc_layout);
	gi.WriteString (string);
}

void PMenu_Next (edict_t *ent)
{
	pmenuhnd_t *hnd;
	int i;
	pmenu_t *p;

	if (!ent->client->menu) {
		gi.dprintf("warning:  ent has no menu\n");
		return;
	}

	hnd = ent->client->menu;

	if (hnd->cur < 0)
		return; // no selectable entries

	i = hnd->cur;
	p = hnd->entries + hnd->cur;
	do {
		i++, p++;
		if (i == hnd->num)
			i = 0, p = hnd->entries;
		if (p->SelectFunc)
			break;
	} while (i != hnd->cur);

	hnd->cur = i;

	PMenu_Update(ent);
	gi.unicast (ent, true);
}

void PMenu_Prev (edict_t *ent)
{
	pmenuhnd_t *hnd;
	int i;
	pmenu_t *p;

	if (!ent->client->menu) {
		gi.dprintf("warning:  ent has no menu\n");
		return;
	}

	hnd = ent->client->menu;

	if (hnd->cur < 0)
		return; // no selectable entries

	i = hnd->cur;
	p = hnd->entries + hnd->cur;
	do {
		if (i == 0) {
			i = hnd->num - 1;
			p = hnd->entries + i;
		} else
			i--, p--;
		if (p->SelectFunc)
			break;
	} while (i != hnd->cur);

	hnd->cur = i;

	PMenu_Update(ent);
	gi.unicast (ent, true);
}

void PMenu_Select (edict_t *ent)
{
	pmenuhnd_t *hnd;
	pmenu_t *p;

	if (!ent->client->menu) {
		gi.dprintf("warning:  ent has no menu\n");
		return;
	}

	hnd = ent->client->menu;

	if (hnd->cur < 0)
		return; // no selectable entries

	p = hnd->entries + hnd->cur;

	if (p->SelectFunc)
		p->SelectFunc(ent, p);
}

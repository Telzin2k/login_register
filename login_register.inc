/* ***************************************************************************
*  Nome do Arquivo: modules/gRP_login_register/login_register.inc            *
*                                                                            *
*  Descrição: Sistema de login e registro do servidor.                       *
*                                                                            *
*         Copyright © GTA Rio Rleplay. Todos os direitos reservados.         *
* ****************************************************************************/

#include <YSI_Coding\y_hooks>

#include "../modules/gRP_login_register/login_textdraw.inc"

//----------------------------------------------------------------------------

stock LogarPlayerDirect(playerid)
{
	LoadPlayerAccount(playerid);
	HidePlayerLogin(playerid);
	SpawnPlayer(playerid);

	Player[playerid][pLogged] = true;
	CallLocalFunction("OnPlayerLoggedIn", "d", playerid);
	return 1;
}

stock ShowPlayerLogin(playerid)
{
    SetSpawnInfo(playerid, 0, 0, 0, 0,0,0,0, 0, 0, 0, 0, 0);
    for(new i = 0; i != 11; i++) TextDrawShowForPlayer(playerid, TelaLogin[i]);

    TogglePlayerSpectating(playerid, true);
	SelectTextDraw(playerid, 0xA9A9A9FF);
	SetPlayerColor(playerid, 0xFFFFFFFF);
    SetPlayerInterior(playerid, 6);
    SetPlayerVirtualWorld(playerid, playerid + MAX_PLAYERS);

	InterpolateCameraPos(playerid, 219.441238, 71.956039, 1005.816406, 219.441238, 71.956039, 1005.816406, 1000);
	InterpolateCameraLookAt(playerid, 219.401382, 66.973983, 1005.395019, 219.401382, 66.973983, 1005.395019, 1000);
	return 1;
}

stock HidePlayerLogin(playerid)
{
	for(new i = 0; i != 11; i++) TextDrawHideForPlayer(playerid, TelaLogin[i]);
	CancelSelectTextDraw(playerid);
	return 1;
}

hook OnPlayerConnect(playerid)
{
//	LogarPlayerDirect(playerid);
	ShowPlayerLogin(playerid);
	return 1;
}

//----------------------------------------------------------------------------

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if(dialogid == DIALOG_LOGIN)
	{
		if(response)
		{
			if(!strlen(inputtext))
			{
				ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD,
				" ", "\n{FC1515}Atenção: A senha que você inseriu está incorreta!\n\n{FFFFFF}Insira sua senha novamente para entrar no servidor.", "Entrar", "Cancelar");
				return 1;
			}

			new String[50];
			format(String, sizeof(String), ACCOUNTS_FOLDER, GetPlayerUser(playerid));

			if(!strcmp(inputtext, DOF2_GetString(String,"Password"), false))
			{
				Player[playerid][pLogged] = true;
				CallLocalFunction("OnPlayerLoggedIn", "d", playerid);

				LoadPlayerAccount(playerid);
				HidePlayerLogin(playerid);

				TogglePlayerSpectating(playerid, false);
				SetPlayerInterior(playerid, 0);
				SetPlayerVirtualWorld(playerid, 0);
				SpawnPlayer(playerid);
				return 1;
			}
			else
			{
				ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD,
				" ", "\n{FC1515}Atenção: A senha que você inseriu está incorreta!\n\n{FFFFFF}Insira sua senha novamente para entrar no servidor.", "Entrar", "Cancelar");
				return 1;
			}
		}
		else
		{
		    SelectTextDraw(playerid, 0x808080FF);
		}
	}
    if(dialogid == DIALOG_REGISTRO)
	{
		if(response)
		{
			if(strlen(inputtext) < 5 || strlen(inputtext) > 25)
			{
				ShowPlayerDialog(playerid, DIALOG_REGISTRO, DIALOG_STYLE_INPUT, " ",
				"\n{FC1515}Aviso: A senha deve conter entre 5 e 25 caracteres!\n\n{FFFFFF}Insira uma senha para registrar sua conta em nosso banco de dados.", "Registrar", "Cancelar");
				return 1;
			}

			new String[50], String2[180];
			format(String, sizeof(String), ACCOUNTS_FOLDER, GetPlayerUser(playerid));

			DOF2_CreateFile(String);
			DOF2_SetString(String, "Login", GetPlayerUser(playerid));
			DOF2_SetString(String, "Password", inputtext);
			DOF2_SetFloat(String, "LastPosX", 229.3599);
			DOF2_SetFloat(String, "LastPosY", -1804.2117);
			DOF2_SetFloat(String, "LastPosZ", 5.6403);
			DOF2_SetFloat(String, "LastPosA", 0.000);
			DOF2_SaveFile();

	        format(String2, sizeof(String2), "\n{FFFFFF}%s, insira sua senha para entrar na cidade.", GetPlayerUser(playerid));
	    	ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, " ", String2, "Entrar", "Cancelar");		
		}
		else
		{
			SelectTextDraw(playerid, 0x808080FF);
			return 1;
		}
	}

	return 1;
}

hook OnPlayerClickTextDraw(playerid, Text:clickedid)
{
 	// Tela de Login e Registro
	new String[60], String2[180];
	format(String, sizeof(String), ACCOUNTS_FOLDER, GetPlayerUser(playerid));

	if(clickedid == TelaLogin[5]) // Entrar
	{
		if(DOF2_FileExists(String))
		{
	        format(String2, sizeof(String2), "\n{FFFFFF}Olá %s, seja bem-vindo(a) novamente,\ninsira sua senha para entrar na cidade.", GetPlayerUser(playerid));
	    	ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, " ", String2, "Entrar", "Cancelar");
	        return 1;
	    }
	    else
	    {
			ShowPlayerDialog(playerid, DIALOG_AVISO, DIALOG_STYLE_MSGBOX, " ", "\n{A9A9A9}Atenção: Sua conta não está registrada em nosso banco de dados!\nClique em 'registrar' para registrar sua conta em nosso banco de dados.", "OK", "");
			return 1;
	    }
	}
	if(clickedid == TelaLogin[6]) // Registrar
	{
		if(!DOF2_FileExists(String))
		{
			ShowPlayerDialog(playerid, DIALOG_REGISTRO, DIALOG_STYLE_INPUT, " ",
			"\n{FFFFFF}Insira uma senha para registrar sua conta em nosso banco de dados.\n\n{A9A9A9}Aviso: A senha deve conter entre 5 e 25 caracteres.", "Registrar", "Cancelar");
			return 1;
		}
		else
		{
			ShowPlayerDialog(playerid, DIALOG_AVISO, DIALOG_STYLE_MSGBOX, " ", "\n{A9A9A9}Atenção: Sua conta já está registrada em nosso banco de dados.\nClique em 'entrar' para entrar em sua conta.", "OK", "");
			return 1;
		}
	}
	if(clickedid == TelaLogin[7]) // Recuperar conta
	{
		return 1;
	}
	if(clickedid == TelaLogin[8]) // Sair
	{
	    GameTextForPlayer(playerid, "$!#{]{]Â¨Â¨**&~w", 1000, 6); // Fechar o GTA do Jogador
	    return 1;
	}
	return 1;
}

/*==LICENSE==*

CyanWorlds.com Engine - MMOG client, server and tools
Copyright (C) 2011  Cyan Worlds, Inc.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Additional permissions under GNU GPL version 3 section 7

If you modify this Program, or any covered work, by linking or
combining it with any of RAD Game Tools Bink SDK, Autodesk 3ds Max SDK,
NVIDIA PhysX SDK, Microsoft DirectX SDK, OpenSSL library, Independent
JPEG Group JPEG library, Microsoft Windows Media SDK, or Apple QuickTime SDK
(or a modified version of those libraries),
containing parts covered by the terms of the Bink SDK EULA, 3ds Max EULA,
PhysX SDK EULA, DirectX SDK EULA, OpenSSL and SSLeay licenses, IJG
JPEG Library README, Windows Media SDK EULA, or QuickTime SDK EULA, the
licensors of this Program grant you additional
permission to convey the resulting work. Corresponding Source for a
non-source form of such a combination shall include the source code for
the parts of OpenSSL and IJG JPEG Library used as well as that of the covered
work.

You can contact Cyan Worlds, Inc. by email legal@cyan.com
 or by snail mail at:
      Cyan Worlds, Inc.
      14617 N Newport Hwy
      Mead, WA   99021

*==LICENSE==*/

#include <Python.h>
#include <string_theory/string>

#include "pyGeometry3.h"
#include "pyKey.h"

#include "pyCritterBrain.h"
#include "pyEnum.h"

#include "plMessage/plAIMsg.h"

///////////////////////////////////////////////////////////////////////////////
//
// AddPlasmaConstantsClasses
//
void pyAIMsg::AddPlasmaConstantsClasses(PyObject *m)
{
    PYTHON_ENUM_START(PtAIMsgType)
    PYTHON_ENUM_ELEMENT(PtAIMsgType, kUnknown, plAIMsg::kAIMsg_Unknown)
    PYTHON_ENUM_ELEMENT(PtAIMsgType, kBrainCreated, plAIMsg::kAIMsg_BrainCreated)
    PYTHON_ENUM_ELEMENT(PtAIMsgType, kArrivedAtGoal, plAIMsg::kAIMsg_ArrivedAtGoal)
    PYTHON_ENUM_END(m, PtAIMsgType)
}

///////////////////////////////////////////////////////////////////////////////

// glue functions
PYTHON_CLASS_DEFINITION(ptCritterBrain, pyCritterBrain);

PYTHON_DEFAULT_NEW_DEFINITION(ptCritterBrain, pyCritterBrain)
PYTHON_DEFAULT_DEALLOC_DEFINITION(ptCritterBrain)

PYTHON_NO_INIT_DEFINITION(ptCritterBrain)

PYTHON_RICH_COMPARE_DEFINITION(ptCritterBrain, obj1, obj2, compareType)
{
    if ((obj1 == Py_None) || (obj2 == Py_None) || !pyCritterBrain::Check(obj1) || !pyCritterBrain::Check(obj2))
    {
        // if they aren't the same type, they don't match, obviously (we also never equal none)
        if (compareType == Py_EQ)
            PYTHON_RCOMPARE_FALSE;
        else if (compareType == Py_NE)
            PYTHON_RCOMPARE_TRUE;
        else
        {
            PyErr_SetString(PyExc_NotImplementedError, "invalid comparison for a ptCritterBrain object");
            PYTHON_RCOMPARE_ERROR;
        }
    }
    pyCritterBrain* brain1 = pyCritterBrain::ConvertFrom(obj1);
    pyCritterBrain* brain2 = pyCritterBrain::ConvertFrom(obj2);
    if (compareType == Py_EQ)
    {
        if ((*brain1) == (*brain2))
            PYTHON_RCOMPARE_TRUE;
        PYTHON_RCOMPARE_FALSE;
    }
    else if (compareType == Py_NE)
    {
        if ((*brain1) != (*brain2))
            PYTHON_RCOMPARE_TRUE;
        PYTHON_RCOMPARE_FALSE;
    }
    PyErr_SetString(PyExc_NotImplementedError, "invalid comparison for a ptCritterBrain object");
    PYTHON_RCOMPARE_ERROR;
}

PYTHON_METHOD_DEFINITION(ptCritterBrain, addReceiver, args)
{
    PyObject* keyObj = nullptr;
    if (!PyArg_ParseTuple(args, "O", &keyObj))
    {
        PyErr_SetString(PyExc_TypeError, "addReceiver expects a ptKey");
        PYTHON_RETURN_ERROR;
    }
    if (!pyKey::Check(keyObj))
    {
        PyErr_SetString(PyExc_TypeError, "addReceiver expects a ptKey");
        PYTHON_RETURN_ERROR;
    }
    pyKey* key = pyKey::ConvertFrom(keyObj);
    self->fThis->AddReceiver(*key);
    PYTHON_RETURN_NONE;
}

PYTHON_METHOD_DEFINITION(ptCritterBrain, removeReceiver, args)
{
    PyObject* keyObj = nullptr;
    if (!PyArg_ParseTuple(args, "O", &keyObj))
    {
        PyErr_SetString(PyExc_TypeError, "removeReceiver expects a ptKey");
        PYTHON_RETURN_ERROR;
    }
    if (!pyKey::Check(keyObj))
    {
        PyErr_SetString(PyExc_TypeError, "removeReceiver expects a ptKey");
        PYTHON_RETURN_ERROR;
    }
    pyKey* key = pyKey::ConvertFrom(keyObj);
    self->fThis->RemoveReceiver(*key);
    PYTHON_RETURN_NONE;
}

PYTHON_METHOD_DEFINITION(ptCritterBrain, getSceneObject, GetSceneObject)
{
    return self->fThis->GetSceneObject();
}

PYTHON_METHOD_DEFINITION_WKEY(ptCritterBrain, addBehavior, args, keywords)
{
    const char* kwlist[] = {"animationName", "behaviorName", "loop", "randomStartPos",
                            "fadeInLen", "fadeOutLen", nullptr};
    ST::string animName;
    ST::string behName;
    char loop = 1, randomStartPos = 1;
    float fadeInLen = 2.f, fadeOutLen = 2.f;
    if (!PyArg_ParseTupleAndKeywords(args, keywords, "O&O&|bbff", const_cast<char **>(kwlist),
                                     PyUnicode_STStringConverter, &animName,
                                     PyUnicode_STStringConverter, &behName,
                                     &loop, &randomStartPos,
                                     &fadeInLen, &fadeOutLen))
    {
        PyErr_SetString(PyExc_TypeError, "addBehavior expects two strings, and optionally two booleans and two floats");
        PYTHON_RETURN_ERROR;
    }

    self->fThis->AddBehavior(animName, behName, loop != 0, randomStartPos != 0, fadeInLen, fadeOutLen);
    PYTHON_RETURN_NONE;
}

PYTHON_METHOD_DEFINITION_WKEY(ptCritterBrain, startBehavior, args, keywords)
{
    const char* kwlist[] = {"behaviorName", "fade", nullptr};
    ST::string behName;
    char fade = 1;
    if (!PyArg_ParseTupleAndKeywords(args, keywords, "O&|b", const_cast<char**>(kwlist),
                                     PyUnicode_STStringConverter, &behName, &fade)) {
        PyErr_SetString(PyExc_TypeError, "startBehavior expects a string, and an optional boolean");
        PYTHON_RETURN_NONE;
    }

    self->fThis->StartBehavior(behName, fade != 0);
    PYTHON_RETURN_NONE;
}

PYTHON_METHOD_DEFINITION(ptCritterBrain, runningBehavior, args)
{
    ST::string behName;
    if (!PyArg_ParseTuple(args, "O&", PyUnicode_STStringConverter, &behName))
    {
        PyErr_SetString(PyExc_TypeError, "runningBehavior expects a string");
        PYTHON_RETURN_ERROR;
    }
    PYTHON_RETURN_BOOL(self->fThis->RunningBehavior(behName));
}

PYTHON_METHOD_DEFINITION(ptCritterBrain, behaviorName, args)
{
    int behavior;
    if (!PyArg_ParseTuple(args, "i", &behavior))
    {
        PyErr_SetString(PyExc_TypeError, "behaviorName expects an integer");
        PYTHON_RETURN_ERROR;
    }
    return PyUnicode_FromSTString(self->fThis->BehaviorName(behavior));
}

PYTHON_METHOD_DEFINITION(ptCritterBrain, animationName, args)
{
    int behavior;
    if (!PyArg_ParseTuple(args, "i", &behavior))
    {
        PyErr_SetString(PyExc_TypeError, "animationName expects an integer");
        PYTHON_RETURN_ERROR;
    }
    return PyUnicode_FromSTString(self->fThis->AnimationName(behavior));
}

PYTHON_METHOD_DEFINITION_NOARGS(ptCritterBrain, curBehavior)
{
    return PyLong_FromLong(self->fThis->CurBehavior());
}

PYTHON_METHOD_DEFINITION_NOARGS(ptCritterBrain, nextBehavior)
{
    return PyLong_FromLong(self->fThis->NextBehavior());
}

PYTHON_METHOD_DEFINITION_NOARGS(ptCritterBrain, idleBehaviorName)
{
    return PyUnicode_FromSTString(self->fThis->IdleBehaviorName());
}

PYTHON_METHOD_DEFINITION_NOARGS(ptCritterBrain, runBehaviorName)
{
    return PyUnicode_FromSTString(self->fThis->RunBehaviorName());
}

PYTHON_METHOD_DEFINITION_WKEY(ptCritterBrain, goToGoal, args, keywords)
{
    const char* kwlist[] = {"newGoal", "avoidingAvatars", nullptr};
    PyObject* goalObj = nullptr;
    char avoidingAvatars = 0;
    if (!PyArg_ParseTupleAndKeywords(args, keywords, "O|b", const_cast<char **>(kwlist),
                                     &goalObj, &avoidingAvatars))
    {
        PyErr_SetString(PyExc_TypeError, "goToGoal expects a ptPoint and an optional boolean.");
        PYTHON_RETURN_ERROR;
    }

    if (!pyPoint3::Check(goalObj))
    {
        PyErr_SetString(PyExc_TypeError, "goToGoal expects a ptPoint and an optional boolean.");
        PYTHON_RETURN_ERROR;
    }
    self->fThis->GoToGoal(pyPoint3::ConvertFrom(goalObj)->fPoint, avoidingAvatars != 0);
    PYTHON_RETURN_NONE;
}

PYTHON_METHOD_DEFINITION_NOARGS(ptCritterBrain, currentGoal)
{
    return self->fThis->CurrentGoal();
}

PYTHON_METHOD_DEFINITION_NOARGS(ptCritterBrain, avoidingAvatars)
{
    PYTHON_RETURN_BOOL(self->fThis->AvoidingAvatars());
}

PYTHON_METHOD_DEFINITION_NOARGS(ptCritterBrain, atGoal)
{
    PYTHON_RETURN_BOOL(self->fThis->AtGoal());
}

PYTHON_METHOD_DEFINITION(ptCritterBrain, setStopDistance, args)
{
    float dist;
    if (!PyArg_ParseTuple(args, "f", &dist))
    {
        PyErr_SetString(PyExc_TypeError, "setStopDistance expects a float");
        PYTHON_RETURN_ERROR;
    }
    self->fThis->StopDistance(dist);
    PYTHON_RETURN_NONE;
}

PYTHON_METHOD_DEFINITION_NOARGS(ptCritterBrain, getStopDistance)
{
    return PyFloat_FromDouble(self->fThis->StopDistance());
}

PYTHON_METHOD_DEFINITION(ptCritterBrain, setSightCone, args)
{
    float dist;
    if (!PyArg_ParseTuple(args, "f", &dist))
    {
        PyErr_SetString(PyExc_TypeError, "setSightCone expects a float");
        PYTHON_RETURN_ERROR;
    }
    self->fThis->SightCone(dist);
    PYTHON_RETURN_NONE;
}

PYTHON_METHOD_DEFINITION_NOARGS(ptCritterBrain, getSightCone)
{
    return PyFloat_FromDouble(self->fThis->SightCone());
}

PYTHON_METHOD_DEFINITION(ptCritterBrain, setSightDistance, args)
{
    float dist;
    if (!PyArg_ParseTuple(args, "f", &dist))
    {
        PyErr_SetString(PyExc_TypeError, "setSightDistance expects a float");
        PYTHON_RETURN_ERROR;
    }
    self->fThis->SightDistance(dist);
    PYTHON_RETURN_NONE;
}

PYTHON_METHOD_DEFINITION_NOARGS(ptCritterBrain, getSightDistance)
{
    return PyFloat_FromDouble(self->fThis->SightDistance());
}

PYTHON_METHOD_DEFINITION(ptCritterBrain, setHearingDistance, args)
{
    float dist;
    if (!PyArg_ParseTuple(args, "f", &dist))
    {
        PyErr_SetString(PyExc_TypeError, "setHearingDistance expects a float");
        PYTHON_RETURN_ERROR;
    }
    self->fThis->HearingDistance(dist);
    PYTHON_RETURN_NONE;
}

PYTHON_METHOD_DEFINITION_NOARGS(ptCritterBrain, getHearingDistance)
{
    return PyFloat_FromDouble(self->fThis->HearingDistance());
}

PYTHON_METHOD_DEFINITION(ptCritterBrain, canSeeAvatar, args)
{
    unsigned long id;
    if (!PyArg_ParseTuple(args, "k", &id))
    {
        PyErr_SetString(PyExc_TypeError, "canSeeAvatar expects an unsigned long");
        PYTHON_RETURN_ERROR;
    }
    PYTHON_RETURN_BOOL(self->fThis->CanSeeAvatar(id));
}

PYTHON_METHOD_DEFINITION(ptCritterBrain, canHearAvatar, args)
{
    unsigned long id;
    if (!PyArg_ParseTuple(args, "k", &id))
    {
        PyErr_SetString(PyExc_TypeError, "canHearAvatar expects an unsigned long");
        PYTHON_RETURN_ERROR;
    }
    PYTHON_RETURN_BOOL(self->fThis->CanHearAvatar(id));
}

PYTHON_METHOD_DEFINITION_NOARGS(ptCritterBrain, playersICanSee)
{
    return self->fThis->PlayersICanSee();
}

PYTHON_METHOD_DEFINITION_NOARGS(ptCritterBrain, playersICanHear)
{
    return self->fThis->PlayersICanHear();
}

PYTHON_METHOD_DEFINITION(ptCritterBrain, vectorToPlayer, args)
{
    unsigned long id;
    if (!PyArg_ParseTuple(args, "k", &id))
    {
        PyErr_SetString(PyExc_TypeError, "vectorToPlayer expects an unsigned long");
        PYTHON_RETURN_ERROR;
    }
    return self->fThis->VectorToPlayer(id);
}

PYTHON_START_METHODS_TABLE(ptCritterBrain)
    PYTHON_METHOD(ptCritterBrain, addReceiver, "Params: key\nTells the brain that the specified key wants AI messages"),
    PYTHON_METHOD(ptCritterBrain, removeReceiver, "Params: key\nTells the brain that the specified key no longer wants AI messages"),
    PYTHON_METHOD(ptCritterBrain, getSceneObject, "Returns the ptSceneObject this brain controls."),
    PYTHON_METHOD_WKEY(ptCritterBrain, addBehavior, "Params: animName, behaviorName, loop = 1, randomStartPos = 1, fadeInLen = 2.0, fadeOutLen = 2.0\n"
        "Adds a new animation to the brain as a behavior with the specified name and parameters. If multiple animations are assigned to the same behavior, "
        "they will be randomly picked from when started."),
    PYTHON_METHOD_WKEY(ptCritterBrain, startBehavior, "Params: behaviorName, fade = 1\nStarts playing the named behavior. If fade is true, it will fade out "
        "the previous behavior and fade in the new one. If false, they will immediately switch."),
    PYTHON_METHOD(ptCritterBrain, runningBehavior, "Params: behaviorName\nReturns true if the named behavior is running."),
    PYTHON_METHOD(ptCritterBrain, behaviorName, "Params: behavior\nReturns the behavior name associated with the specified integral behavior."),
    PYTHON_METHOD(ptCritterBrain, animationName, "Params: behavior\nReturns the animation name associated with the specified integral behavior."),
    PYTHON_METHOD_NOARGS(ptCritterBrain, curBehavior, "Returns the current integral behavior the brain is running."),
    PYTHON_METHOD_NOARGS(ptCritterBrain, nextBehavior, "Returns the behavior the brain will be switching to next frame. (-1 if no change)"),
    PYTHON_METHOD_NOARGS(ptCritterBrain, idleBehaviorName, "Returns the name of the brain's idle behavior."),
    PYTHON_METHOD_NOARGS(ptCritterBrain, runBehaviorName, "Returns the name of the brain's run behavior."),
    PYTHON_METHOD_WKEY(ptCritterBrain, goToGoal, "Params: newGoal, avoidingAvatars = 0\nTells the brain to start running towards the specified location, "
        "avoiding avatars it can see or hear if told to."),
    PYTHON_METHOD_NOARGS(ptCritterBrain, currentGoal, "Returns the current ptPoint that the brain is running towards."),
    PYTHON_METHOD_NOARGS(ptCritterBrain, avoidingAvatars, "Are we currently avoiding avatars while pathfinding?"),
    PYTHON_METHOD_NOARGS(ptCritterBrain, atGoal, "Are we currently are our final destination?"),
    PYTHON_METHOD(ptCritterBrain, setStopDistance, "Params: dist\nSet how far away from the goal we should be when we are considered there and stop running."),
    PYTHON_METHOD_NOARGS(ptCritterBrain, getStopDistance, "Returns how far away from the goal we could be and still be considered there."),
    PYTHON_METHOD(ptCritterBrain, setSightCone, "Params: radians\nSet how wide the brain's field of view is in radians. Note that it is the total angle of the "
        "cone, half on one side of the brain's line of sight, half on the other."),
    PYTHON_METHOD_NOARGS(ptCritterBrain, getSightCone, "Returns the width of the brain's field of view in radians."),
    PYTHON_METHOD(ptCritterBrain, setSightDistance, "Params: dist\nSet how far away the brain can see."),
    PYTHON_METHOD_NOARGS(ptCritterBrain, getSightDistance, "Returns how far the brain can see."),
    PYTHON_METHOD(ptCritterBrain, setHearingDistance, "Params: dist\nSet how far away the brain can hear (360 degree field of hearing)."),
    PYTHON_METHOD_NOARGS(ptCritterBrain, getHearingDistance, "Returns how far away the brain can hear."),
    PYTHON_METHOD(ptCritterBrain, canSeeAvatar, "Params: avatarID\nReturns whether this brain can see the avatar with the specified id."),
    PYTHON_METHOD(ptCritterBrain, canHearAvatar, "Params: avatarID\nReturns whether this brain can hear the avatar with the specified id."),
    PYTHON_METHOD_NOARGS(ptCritterBrain, playersICanSee, "Returns a list of player ids which this brain can see."),
    PYTHON_METHOD_NOARGS(ptCritterBrain, playersICanHear, "Returns a list of player ids which this brain can hear."),
    PYTHON_METHOD(ptCritterBrain, vectorToPlayer, "Params: avatarID\nReturns the vector between us and the specified player."),
PYTHON_END_METHODS_TABLE;

// Type structure definition
#define ptCritterBrain_AS_NUMBER    PYTHON_NO_AS_NUMBER
#define ptCritterBrain_AS_SEQUENCE  PYTHON_NO_AS_SEQUENCE
#define ptCritterBrain_AS_MAPPING   PYTHON_NO_AS_MAPPING
#define ptCritterBrain_STR          PYTHON_NO_STR
#define ptCritterBrain_GETATTRO     PYTHON_NO_GETATTRO
#define ptCritterBrain_SETATTRO     PYTHON_NO_SETATTRO
#define ptCritterBrain_RICH_COMPARE PYTHON_DEFAULT_RICH_COMPARE(ptCritterBrain)
#define ptCritterBrain_GETSET       PYTHON_NO_GETSET
#define ptCritterBrain_BASE         PYTHON_NO_BASE
PLASMA_CUSTOM_TYPE(ptCritterBrain, "Object to manipulate critter brains");

// required functions for PyObject interoperability
PyObject* pyCritterBrain::New(plAvBrainCritter* brain)
{
    ptCritterBrain *newObj = (ptCritterBrain*)ptCritterBrain_type.tp_new(&ptCritterBrain_type, nullptr, nullptr);
    newObj->fThis->fBrain = brain;
    return (PyObject*)newObj;
}

PYTHON_CLASS_CHECK_IMPL(ptCritterBrain, pyCritterBrain)
PYTHON_CLASS_CONVERT_FROM_IMPL(ptCritterBrain, pyCritterBrain)

///////////////////////////////////////////////////////////////////////////
//
// AddPlasmaClasses - the python module definitions
//
void pyCritterBrain::AddPlasmaClasses(PyObject *m)
{
    PYTHON_CLASS_IMPORT_START(m);
    PYTHON_CLASS_IMPORT(m, ptCritterBrain);
    PYTHON_CLASS_IMPORT_END(m);
}

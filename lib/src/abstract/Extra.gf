--1 More syntax rules

-- This module defines syntax rules that are not implemented in all
-- languages, but in more than one, so that it makes sense to offer a
-- common API.

abstract Extra = Cat ** {

  fun
    GenNP       : NP -> Quant ;       -- this man's
    ComplBareVS : VS -> S -> VP ;     -- know you go

    StrandRelSlash   : RP -> ClSlash -> RCl ;   -- that he lives in
    EmptyRelSlash    : ClSlash -> RCl ;   -- he lives in
    StrandQuestSlash : IP -> ClSlash -> QCl ;   -- whom does John live with

-- $VP$ conjunction, which has different fragments implemented in
-- different languages - never a full $VP$, though.

  cat
    VPI ;
    [VPI] {2} ;

  fun
    MkVPI : VP -> VPI ;
    ConjVPI : Conj -> [VPI] -> VPI ;
    ComplVPIVV : VV -> VPI -> VP ;

  -- new 4/12/2009
  cat
    VPS ;
    [VPS] {2} ;

  fun
    MkVPS : Temp -> Pol -> VP -> VPS ;
    ConjVPS : Conj -> [VPS] -> VPS ;
    PredVPS : NP -> VPS -> S ;

  -- 9/4/2010

  fun
    ProDrop : Pron -> Pron ;  -- unstressed subject pronoun becomes []: "(io) sono stanco"
    ICompAP : AP -> IComp ;   -- "how old"
    IAdvAdv : Adv -> IAdv ;   -- "how often"

    CompIQuant : IQuant -> IComp ; -- which (is it) [agreement to NP]

    PrepCN : Prep -> CN -> Adv ;   -- by accident [Prep + CN without article]

  -- fronted/focal constructions, only for main clauses

  cat
    Foc ;

  fun
    FocObj : NP  -> ClSlash -> Foc ;   -- her I love
    FocAdv : Adv -> Cl      -> Foc ;   -- today I will sleep
    FocAdV : AdV -> Cl      -> Foc ;   -- never will I sleep
    FocAP  : AP  -> NP      -> Foc ;   -- green was the tree
    FocNeg : Cl             -> Foc ;   -- not is he here
    FocVP  : VP  -> NP      -> Foc ;   -- love her I do
    FocVV  : VV -> VP -> NP -> Foc ;   -- to love her I want
    
    UseFoc : Temp -> Pol -> Foc -> Utt ;

  cat
    [CN] {2} ;
  fun
    ConjCN : Conj -> [CN] -> CN ; -- (every) man and woman

    PartVP : VP -> AP ; -- looking at Mary

  cat 
    QVP ;          -- buy what where
    [IAdv] {2} ;   -- when and where
  fun
    ComplSlashIP  : VPSlash -> IP -> QVP ;   -- buys what 
    AdvQVP        : VP  ->   IAdv -> QVP ;   -- lives where 
    AddAdvQVP     : QVP ->   IAdv -> QVP ;   -- buys what where 

    QuestQVP      : IP -> QVP -> QCl ;       -- who buys what where

    ConjIAdv      : Conj -> [IAdv] -> IAdv ; -- when, where and with whom

    AdvAP : AP -> Adv -> AP ;  -- hungry as a wolf

    UseCopula : VP ;
}

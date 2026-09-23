#ifndef _SDLCOMPAT_H
#define _SDLCOMPAT_H

#include <SDL3/SDL.h>

// SDL3 moved the renderer's draw calls from int-based SDL_Rect to float-based SDL_FRect
// (SDL_RenderCopy -> SDL_RenderTexture, SDL_RenderDrawRect -> SDL_RenderRect, etc.), and
// SDL_GetRGBA now wants a SDL_PixelFormatDetails* instead of a bare SDL_PixelFormat.
// This codebase's rects are integer tile/pixel coordinates throughout, so rather than
// touch every one of the ~150 call sites, these keep the old SDL2-shaped signatures and
// convert at the boundary.

inline bool DA2_RenderCopy(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect){
  SDL_FRect fsrc, fdst;
  const SDL_FRect *ps = NULL, *pd = NULL;
  if(srcrect){ fsrc.x=(float)srcrect->x; fsrc.y=(float)srcrect->y; fsrc.w=(float)srcrect->w; fsrc.h=(float)srcrect->h; ps=&fsrc; }
  if(dstrect){ fdst.x=(float)dstrect->x; fdst.y=(float)dstrect->y; fdst.w=(float)dstrect->w; fdst.h=(float)dstrect->h; pd=&fdst; }
  return SDL_RenderTexture(renderer, texture, ps, pd);
}

inline bool DA2_RenderFillRect(SDL_Renderer* renderer, const SDL_Rect* rect){
  if(!rect) return SDL_RenderFillRect(renderer, NULL);
  SDL_FRect f = { (float)rect->x, (float)rect->y, (float)rect->w, (float)rect->h };
  return SDL_RenderFillRect(renderer, &f);
}

inline bool DA2_RenderDrawRect(SDL_Renderer* renderer, const SDL_Rect* rect){
  if(!rect) return SDL_RenderRect(renderer, NULL);
  SDL_FRect f = { (float)rect->x, (float)rect->y, (float)rect->w, (float)rect->h };
  return SDL_RenderRect(renderer, &f);
}

inline void DA2_GetRGBA(Uint32 pixel, SDL_PixelFormat format, Uint8* r, Uint8* g, Uint8* b, Uint8* a){
  SDL_GetRGBA(pixel, SDL_GetPixelFormatDetails(format), NULL, r, g, b, a);
}

#endif

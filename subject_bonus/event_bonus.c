/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 19:47:42 by hyungdki          #+#    #+#             */
/*   Updated: 2023/07/04 00:14:42 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf_bonus.h"

static void	image_move(int keycode, t_fdf *fdf);
static void	image_rotate(int keycode, t_fdf *fdf);
static void	move_speed(int keycode, t_fdf *fdf);

int	press_cross_on_window_frame(t_fdf *fdf)
{
	free_2d_array((void *)fdf->map_ptr->map);
	mlx_destroy_window(fdf->mlx_ptr, fdf->win_ptr);
	exit(0);
}

int	key_event(int keycode, t_fdf *fdf)
{
	if(keycode == LEFT_KEY || keycode == RIGHT_KEY || keycode == DOWN_KEY
		|| keycode == UP_KEY)
		image_move(keycode, fdf);
	else if (keycode == A_KEY || keycode == S_KEY || keycode == D_KEY
		|| keycode == W_KEY)
		image_rotate(keycode, fdf);
	else if (keycode == PL_KEY || keycode == MN_KEY || keycode == SBO_KEY
		|| keycode == SBC_KEY)
		move_speed(keycode, fdf);
	else if (keycode == ESC_KEY)
	{
		free_2d_array((void *)fdf->map_ptr->map);
		mlx_destroy_window(fdf->mlx_ptr, fdf->win_ptr);
		exit(0);
	}
	return (0);
}

static void	image_move(int keycode, t_fdf *fdf)
{
	if(keycode == LEFT_KEY)
		fdf->x2d_move += fdf->move_speed;
	else if(keycode == RIGHT_KEY)
		fdf->x2d_move -= fdf->move_speed;
	if(keycode == UP_KEY)
		fdf->y2d_move += fdf->move_speed;
	else if(keycode == DOWN_KEY)
		fdf->y2d_move -= fdf->move_speed;
	fdf->loc_change = TRUE;
}

static void	move_speed(int keycode, t_fdf *fdf)
{
	if (keycode == PL_KEY && fdf->move_speed < 100)
		fdf->move_speed += 1;
	else if (keycode == MN_KEY && fdf->move_speed > 0)
		fdf->move_speed -= 1;
	else if (keycode == SBC_KEY && fdf->rot_speed < 180)
		fdf->rot_speed += 1;
	else if (keycode == SBO_KEY && fdf->rot_speed > 1)
		fdf->rot_speed -= 1;
}

static void image_rotate(int keycode, t_fdf *fdf)
{
	if (keycode == S_KEY)
	{
		fdf->dtheta += fdf->rot_speed;
		if (fdf->dtheta >= 360)
			fdf->dtheta -= 360;
	}
	else if (keycode == W_KEY)
	{
		fdf->dtheta -= fdf->rot_speed;
		if (fdf->dtheta < 0)
			fdf->dtheta += 360;
	}
	if (keycode == D_KEY)
	{
		fdf->dphi += fdf->rot_speed;
		if (fdf->dphi >= 360)
			fdf->dphi -= 360;
	}
	else if (keycode == A_KEY)
	{
		fdf->dphi -= fdf->rot_speed;
		if (fdf->dphi < 0)
			fdf->dphi += 360;
	}
	fdf->rotate_change = TRUE;
}

int mouse_click(int button, int x, int y, t_fdf *fdf)
{
	if ((0 <= x && x < fdf->win_size_x) && (0 <= y && y < fdf->win_size_y))
	{
		if (button == BUT1_KEY)
		{
			fdf->l_mouse_clk = TRUE;
			fdf->old_x = x;
			fdf->old_y = y;
		}
		else if (button == BUT2_KEY)
		{
			fdf->r_mouse_clk = TRUE;
			fdf->old_x = x;
			fdf->old_y = y;
		}
		if (button == SCROLLUP_KEY && fdf->map_ptr->basic_len_index < 100)
		{
			int dx1;
			int dx2;
			int dy1;
			int dy2;
			int center_x;
			int center_y;
			double pre_len;

			pre_len = fdf->map_ptr->basic_len;
			fdf->map_ptr->basic_len_index += 1;
			fdf->map_ptr->basic_len = pow(ZOOM_VALUE, fdf->map_ptr->basic_len_index);
			center_x = fdf->map_ptr->midpoint_x2d + fdf->x2d_move;
			center_y = fdf->map_ptr->midpoint_y2d + fdf->y2d_move;
			dx1 = abs(x  - center_x);
			dx2 = (int)(dx1 * (fdf->map_ptr->basic_len / pre_len) + 0.5);
			dy1 = abs(y  - center_y);
			dy2 = (int)(dy1 * (fdf->map_ptr->basic_len / pre_len) + 0.5);
			if (x > center_x)
				fdf->x2d_move -= (dx2 - dx1);
			else 
				fdf->x2d_move += (dx2 - dx1);
			if (y > center_y)
				fdf->y2d_move += (dy2 - dy1);
			else 
				fdf->y2d_move -= (dy2 - dy1);
			printf("x move : %d, y move : %d\n", fdf->x2d_move, fdf->y2d_move);
			fdf->zoom_change = TRUE;
		}
		else if (button == SCROLLDOWN_KEY && fdf->map_ptr->basic_len_index > -10)
		{
			int dx1;
			int dx2;
			int dy1;
			int dy2;
			int center_x;
			int center_y;
			double pre_len;

			pre_len = fdf->map_ptr->basic_len;
			fdf->map_ptr->basic_len_index -= 1;
			fdf->map_ptr->basic_len = pow(ZOOM_VALUE, fdf->map_ptr->basic_len_index);
			center_x = fdf->map_ptr->midpoint_x2d + fdf->x2d_move;
			center_y = fdf->map_ptr->midpoint_y2d + fdf->y2d_move;
			dx1 = abs(x  - center_x);
			dx2 = (int)(dx1 * (fdf->map_ptr->basic_len / pre_len) + 0.5);
			dy1 = abs(y  - center_y);
			dy2 = (int)(dy1 * (fdf->map_ptr->basic_len / pre_len) + 0.5);
			if (x > center_x)
				fdf->x2d_move += (dx1 - dx2);
			else 
				fdf->x2d_move -= (dx1 - dx2);
			if (y > center_y)
				fdf->y2d_move -= (dy1 - dy2);
			else 
				fdf->y2d_move += (dy1 - dy2);
			printf("x move : %d, y move : %d\n", fdf->x2d_move, fdf->y2d_move);
			fdf->zoom_change = TRUE;
		}
	}
	return (0);
}

int	mouse_release(int button, int x, int y, t_fdf *fdf)
{
	x = x + y;
	if (button == BUT1_KEY)
		fdf->l_mouse_clk = FALSE;
	if (button == BUT2_KEY)
		fdf->r_mouse_clk = FALSE;
	return (0);
}

int	mouse_move(int x, int y, t_fdf *fdf)
{
	int	dx;
	int	dy;
	
	if ((0 <= x && x < fdf->win_size_x) && (0 <= y && y < fdf->win_size_y))
	{
		if (fdf->l_mouse_clk)
		{
			dx = x - fdf->old_x;
			dy = y - fdf->old_y;
			if (dx != 0)
			{
				fdf->x2d_move += dx;
				fdf->loc_change = TRUE;
			}
			if (dy != 0)
			{
				fdf->y2d_move += dy;
				fdf->loc_change = TRUE;
			}
			fdf->old_x = x;
			fdf->old_y = y;
		}
		else if (fdf->r_mouse_clk)
		{
			dx = x - fdf->old_x;
			dy = y - fdf->old_y;
			if (dy > 0)
				image_rotate(S_KEY,fdf);
			else if (dy < 0)
				image_rotate(W_KEY,fdf);
			if (225 < fdf->dtheta || fdf->dtheta <= 45)
			{	
				if (dx > 0)
					image_rotate(A_KEY,fdf);
				else if (dx < 0)
					image_rotate(D_KEY,fdf);
			}
			else
			{
				if (dx > 0)
					image_rotate(D_KEY,fdf);
				else if (dx < 0)
					image_rotate(A_KEY,fdf);	
			}
			fdf->old_x = x;
			fdf->old_y = y;
		}
	}
	return (0);
}
